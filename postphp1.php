<?php
    $error = $loginError = "";
    $username = $stream = "";

    exec("\"./IDL\" index.wpml > index.php");

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        /*verify user is logged in*/
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

        if(!empty($loginError)) {
            echo "<a href=\"index.php\">Back to Login</a><br>";
            die("$loginEror Please login.<br>");
        }
        else {
            exec("\"./IDL\" view.wpml > view.php");
            exec("\"./IDL\" post.wpml > post.php");
        }

        if (isset($_POST["submit"])) {
            if (empty($_POST["messageForPost"])) {
                $error .= "Blank message provided. ";
            }
            else {
                $messageForPost = $_POST["messageForPost"];
            }

            if (empty($_POST["streamForPost"])) {
                $error .= "No stream provided. ";
            }
            else {
                $streamForPost = $_POST["streamForPost"];
            }

            if (empty($error)) {
