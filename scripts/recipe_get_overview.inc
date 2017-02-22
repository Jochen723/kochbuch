<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 02.02.17
 * Time: 18:23
 */

function recipe_get_overview_overview() {

    $test = recipe_get_overview_get_recipe();

    $form['test']['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $test
    );

    $form['test']['note'] = array(
        '#type' => 'textarea',
        '#default_value' => recipe_get_overview_get_notes(),
    );

    $form['blablallv'] = array(
        '#type' => 'submit',
        '#value' => 'Noitzen speichern',
        '#submit' => array('recipe_get_overview_save_note')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Zurück zur Übersicht',
        '#submit' => array('recipe_get_overview_back_to_overview')
    );

    $form['submit2'] = array(
        '#type' => 'submit',
        '#value' => 'Bearbeiten',
        '#submit' => array('recipe_get_overview_change_recipe')
    );

    return $form;
}

function recipe_get_overview_back_to_overview() {
    drupal_goto("rezeptuebersicht");
}

function recipe_get_overview_get_recipe() {
    global $base_url;
    $path = $base_url."/sites/default/files/";

    $recipe_id = arg(1);
    $sql = "SELECT * FROM {recipe_ingredient} AS rec1 INNER JOIN {recipe} AS rec2 ON rec1.recipe_id = rec2.recipe_id WHERE rec1.recipe_id = ".$recipe_id;
    $result = db_query($sql);
    $zutatenArray = array();
    $test = "";
    $preparation = "";
    $difficulty = "";
    $work_time = "";
    $number = "";
    foreach ($result as $r) {
        array_push($zutatenArray, $r);
        $preparation = $r->preparation;
        $number = $r->number_of_persons;
    }

    $html = "<h1>".$zutatenArray[0]->title."</h1>";
    $difficulty = $zutatenArray[0]->difficulty;
    $work_time = $zutatenArray[0]->work_time;
    $zutatenArray = recipe_change_number_of_persons($zutatenArray, $number);
    foreach ($zutatenArray as $zut) {
        $test .= $zut->quantity . " " . $zut->measure . " <a href='/kochbuch/suche_nach_zutat/$zut->ingredient'>$zut->ingredient</a><br>";
    }

    global $base_url;

    $minus = arg(2)-1;
    if($minus <0) {
        $minus = 0;
    }
    $plus = arg(2)+1;


    $test .= "<br><a href='".$base_url."/rezeptansicht/".arg(1)."/".$minus."'>Weniger</a> ".arg(2)." Personen <a href='".$base_url."/rezeptansicht/".arg(1)."/".$plus."'>Mehr</a>";

    $sql = "SELECT * FROM {recipe_data} WHERE recipe_id = ".$recipe_id;
    $result2 = db_query($sql);
    $path2 = "Beispielbild.jpeg";

    foreach ($result2 as $r2) {
        $path2 = substr($r2->data_path, 9);
        $html .=  '<img src="'.$path.$path2.'" alt="Smiley face" height="300" width="300">';
    }

    $html .= "<br><br>";
    $html .= "<h2>Zutaten</h2>";
    $html .= $test;

    $html .= "<br><br>";
    $html .= "<h2>Zubereitung</h2>";
    $html .= $preparation;

    $html = '<html>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Website</title>


<style>
.center  { text-align: center; }
      .justify { text-align: justify; }
	#container {
		background:transparent;
	}
	#links{
		margin-right: 300px;
		background:transparent;

	}
	#links2{
		margin-right: 100px;
		background:transparent;

	}
	#rechts{
		float: right;
		width: 300px;
	}

</style>

</head>
<body>
<h1>'.$zutatenArray[0]->title.'</h1>

<div id="container">
	<div id="rechts">
		<h2>Zutaten</h2> '.$test.'
	</div>
	<div id="links">
		<img src="'.$path.$path2.'" alt="Smiley face" height="300" width="300">
		<div style="clear:both"></div>
	</div>
</div>

<div id="container">
	<div id="links2">
		<h2>Zubereitung</h2>
		<section class="center">
      <p class="justify">'.$preparation.'</p>
    </section>
		<div style="clear:both"></div>
	</div>
</div>
<div id="container">
	<div id="links2">
		<h3>Arbeitszeit: '.$work_time.' Minute(n)</h3>
		<h3>Schwierigkeitsgrad: '.$difficulty.'</h3>
		<section class="center">
    </section>
		<div style="clear:both"></div>
	</div>
</div>

<h2>Notizen</h2>
</body>
</html>
';
    return $html;
}

function recipe_get_overview_change_recipe() {
    $recipe_id = arg(1);
    drupal_goto("rezept_bearbeiten/".$recipe_id);
}

function recipe_get_overview_get_notes() {
    $recipe_id = arg(1);
    $note = "";
    $sql = "SELECT * FROM {recipe} WHERE {recipe_id} = ".$recipe_id;
    $result = db_query($sql);
    foreach ($result as $item) {
        $note = $item->note;
    }

    return $note;
}

function recipe_get_overview_save_note($form, &$form_state) {
    $recipe_id = arg(1);
    $note = $form_state['values']['note'];

    $sql = "UPDATE {recipe} SET note = '".$note."' WHERE recipe_id = ".$recipe_id;
    db_query($sql);
}

function recipe_change_number_of_persons($array, $number) {
    $anzahl = arg(2);
    $num = $number;
    $a = $array;

    foreach ($a as $r) {
        $r->quantity = round($r->quantity/$num*$anzahl,2);
    }

    return $a;
}