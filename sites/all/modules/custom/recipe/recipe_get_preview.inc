<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 23.01.17
 * Time: 18:33
 */

function recipe_get_preview() {
    $recipe_id = arg(1);
    $sql = "SELECT * FROM {recipe_ingredient} AS rec1 INNER JOIN {recipe} AS rec2 ON rec1.recipe_id = rec2.recipe_id WHERE rec1.recipe_id = ".$recipe_id;
    $result = db_query($sql);
    $zutatenArray = array();
    foreach ($result as $r) {
        array_push($zutatenArray, $r);
    }

    $html = "<h1>".$zutatenArray[0]->title."</h1>";
    foreach ($zutatenArray as $zut) {
        $html .= $zut->quantity . " " . $zut->measure . " " . $zut->ingredient . "<br>";
    }
    
    return $html;
}