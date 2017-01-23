<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 23.01.17
 * Time: 18:33
 */

function recipe_get_preview() {

    $path = "http://localhost/kochbuch/sites/default/files/";

    $recipe_id = arg(1);
    $sql = "SELECT * FROM {recipe_ingredient} AS rec1 INNER JOIN {recipe} AS rec2 ON rec1.recipe_id = rec2.recipe_id WHERE rec1.recipe_id = ".$recipe_id;
    $result = db_query($sql);
    $zutatenArray = array();
    $test = "";
    $preparation = "";
    foreach ($result as $r) {
        array_push($zutatenArray, $r);
        $preparation = $r->preparation;
    }

    $html = "<h1>".$zutatenArray[0]->title."</h1>";
    foreach ($zutatenArray as $zut) {
        $test .= $zut->quantity . " " . $zut->measure . " " . $zut->ingredient . "<br>";
    }

    $sql = "SELECT * FROM {recipe_data} WHERE recipe_id = ".$recipe_id;
    $result2 = db_query($sql);

    foreach ($result2 as $r2) {
        $path2 = substr($r2->data_path, 9);
        $html .=  '<img src="'.$path.$path2.'" alt="Smiley face" height="300" width="300">'; ;
    }
    $html .= "<br><br>";
    $html .= "<h2>Zutaten</h2>";
    $html .= $test;
    $html .= "<br><br>";
    $html .= "<h2>Zubereitung</h2>";
    $html .= $preparation;

    return $html;
}