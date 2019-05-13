<?php
function content($file) {
    if (file_exists($file) === false) {
        echo "<h3>Your kid is a good kid ;), he didn't type / read / saw anything forbidden</h3>";
        return "none";
    }
    $myfile = fopen($file, 'r');
    
    $content = explode("\n",fread($myfile, filesize($file)));
    $max = 0;
    for ($i = 0; $i < count($content); $i++) {
        $content[$i] = explode(",", $content[$i]);
    }
    fclose($myfile);
    return $content;
}
?>