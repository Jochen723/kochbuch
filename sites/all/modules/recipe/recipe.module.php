<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 23.01.17
 * Time: 17:33
 */

function recipe_menu() {

    $items['rezept_erstellen_allgemein'] = array(
        'title' => 'Rezept erstellen',
        'access callback' => true,
        'page callback' => 'drupal_get_form',
        'page arguments' => array('recipe_add_general_information_form'),
    );

    return $items;
}

function recipe_add_general_information_form() {

    $form['bodyTextField'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('please type in a regular mail address:'),
        '#default_value' => t('')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'register to newsletter',
        '#submit' => array('register_to_newsletter_register')
    );

    return $form;
}
