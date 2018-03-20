<?php
    $error = $loginError = "";
    $username = $stream = "";

    exec("\"./IDL\" index.wpml > index.php");

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        /*user login data*/
        if (empty($_POST["username"])) {
            $loginError .= "No user logged in. ";
        }
        else {
            $username = $_POST["username"];
        }
            
        if (empty($_POST["stream"])) {
            $loginError .= "No stream selected. ";
        }
        else {
            $stream = $_POST["stream"];
        }

        exec("\"./IDL\" addAuthor.wpml > addAuthor.php");

        /*form submitted to add or remove author*/
        if (isset($_POST["submit"])) {
            if (empty($_POST["author"])) {
                $error .= "Author not provided. ";
            }
            else {
                $author = $_POST["author"];
            }

            if (empty($_POST["streamList"])) {
                $error .= "Streams not provided. ";
            }
            else {
                $streamList = $_POST["streamList"];
            }
            if (empty($_POST["authorAction"])) {
                $error .= "Action to perform not selected. ";
            }
            else {
                $authorAction = $_POST["authorAction"];
            }

            if (empty($error)) {
                if ($authorAction == "add") {
