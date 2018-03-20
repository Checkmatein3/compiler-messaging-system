<?php 
    $username = "";
    
    exec("\"./IDL\" index.wpml > index.php");

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        if (empty($_POST["username"])) {
            echo "<a href=\"index.php\">Back to Login</a><br>";
            die("No user logged in.");
        }
        else {
            $username = $_POST["username"];
        }

        exec("\"./IDL\" stream.wpml > stream.php");
        exec("\"./IDL\" view.wpml > view.php");
