<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 24.01.17
 * Time: 21:29
 */

function recipe_change_information_form() {

    $array = get_recipe_data();

    $form['main'] = array(

        '#type' => 'fieldset',
        '#title' => t('Informationen zum Rezept'),
    );

    $form['main']['title'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('Name des Rezepts:'),
        '#required' => TRUE,
        '#default_value' => t($array->title)
    );

    $form['main']['howto'] = array(
        '#type' => 'textarea',
        '#required' => TRUE,
        '#title' => t('Zubereitung:'),
        '#default_value' => t($array->preparation)
    );

    $form['main']['time'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('Arbeitszeit'),
        '#required' => TRUE,
        '#size' => 3,
        '#field_suffix' => 'Minuten',
        '#default_value' => t($array->work_time)
    );

    $form["main"]["difficulty"] = array(
        "#type" => "select",
        "#title" => t("Schwierigkeitsgrad"),
        '#required' => TRUE,
        "#options" => array("leicht" => "leicht", "mittel" => "mittel", "schwer" => "schwer"),
        "#default_value" => t($array->difficulty),
    );

    $form['main']['file'] = array(
        '#type' => 'file',
        '#name' => 'files[]',
        '#title' => t('Foto-Upload'),
        '#attributes' => array('multiple' => 'multiple'),
    );

    $form['options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Zutatenliste (max. 12)'),
        '#attributes' => array('class' => array('container-inline')),
    );
    $array = recipe_get_units();

    $inter = get_recipe_ingredient_data();


    for ($i = 0; $i<=11; $i++) {

        $form['options']['operation' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#size' => 4,
            '#title' => t('Name des Rezepts:'),
            '#title_display' => 'invisible',
            '#default_value' => t($inter[$i]['quantity'])
        );

        $form['options']['operation2' . $i] = array(
            '#type' => 'select',
            '#title' => t('Operation'),
            '#title_display' => 'invisible',
            '#options' => $array,
            '#default_value' => 'approve',
        );

        $form['options']['operation3' . $i] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#size' => 40,
            '#title' => t('Name des Rezepts:'),
            '#title_display' => 'invisible',
            '#default_value' => t($inter[$i]['ingredient'])
        );

        $test = "<br>";
        $form['options']['form_item2' . $i] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test
        );
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('recipe_save_recipe_changes')
    );

    $form['submit2'] = array(
        '#type' => 'submit',
        '#value' => 'Löschen',
        '#submit' => array('recipe_delete_recipe_changes')
    );

    return $form;
}

function recipe_delete_recipe_changes() {
    $recipe_id = arg(1);
    $sql = "DELETE FROM {recipe} WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    $sql = "DELETE FROM {recipe_ingredient} WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    $sql = "DELETE FROM {recipe_data} WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    $sql = "DELETE FROM {recipe_cooked} WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    drupal_goto("rezeptuebersicht");
    drupal_set_message("Rezept wurde gelöscht");
}

function get_recipe_data() {
    $recipe_id = arg(1);
    $recipe = "";

    $sql = "SELECT * FROM {recipe} WHERE recipe_id = ".$recipe_id;
    $result = db_query($sql);

    foreach ($result as $r) {
        $recipe = $r;
    }

    return $recipe;
}

function get_recipe_ingredient_data() {
    $recipe_id = arg(1);
    $ingredients = array();
    $temp = "";

    $sql = "SELECT * FROM {recipe_ingredient} WHERE recipe_id = ".$recipe_id;
    $result = db_query($sql);

    foreach ($result as $r) {
        $size = sizeof($ingredients);
        $ingredients[$size]['ingredient']=$r->ingredient;
        $ingredients[$size]['measure']=$r->measure;
        $ingredients[$size]['quantity']=$r->quantity;
        $temp = $r;
    }


    while (sizeof($ingredients) <= 12) {
        $size = sizeof($ingredients);
        $ingredients[$size]['ingredient']="";
        $ingredients[$size]['measure']="";
        $ingredients[$size]['quantity']="";
    }



    return $ingredients;
}

function recipe_save_recipe_changes($form, &$form_state) {
    $recipe_id = arg(1);



    $titel = $form_state['values']['title'];
    $preparation = $form_state['values']['howto'];
    $work_time = $form_state['values']['time'];
    $difficulty = $form_state['values']['difficulty'];

    $sql = "UPDATE {recipe} SET title = '".$titel."',preparation = '".$preparation."',work_time = ".$work_time.", difficulty = '".$difficulty. "' WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    $array = recipe_get_units();

    $sql = "DELETE FROM {recipe_ingredient} WHERE recipe_id = ".$recipe_id;
    db_query($sql);

    for ($i = 0; $i<=11; $i++) {
        $eins = $form_state['values']['operation' . $i];
        $zwei = $array[$form_state['values']['operation2' . $i]];
        $drei = $form_state['values']['operation3' . $i];

        if(strlen($eins) > 0 && strlen($zwei) > 0 && strlen($drei) > 0) {
            $sql = "INSERT INTO {recipe_ingredient} VALUES (".$recipe_id.",'".$drei."',".$eins.",'".$zwei."')";
            db_query($sql);
        }
    }

    $num_files = count($_FILES['files']['name']);

    for ($i = 0; $i < $num_files; $i++) {
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(5 * 1024 * 1024),
        ));
        if ($file) {
            if ($file = file_move($file, 'public://')) {
                $file->status = FILE_STATUS_PERMANENT;
                $sql = "UPDATE {file_managed} SET status = 1 WHERE fid > 0";
                db_query($sql);
                $path = substr($file->uri, 9);
                $sql = "DELETE FROM {recipe_data} WHERE recipe_id = ".$recipe_id;
                db_query($sql);

                $sql = "INSERT INTO {recipe_data} VALUES (".$recipe_id.",'".$file->uri."')";
                $form_state['values']['file'][$i] = $file;
                db_query($sql);
            }
        } else {
            drupal_set_message("NOO");

        }
    }

    drupal_goto("rezeptuebersicht");
}