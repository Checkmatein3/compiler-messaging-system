<?php
    $error = $loginError = "";
    $currentStream = $username = $stream = $messageToDisplay = "";
    $sortMode = "sortByDate";
    $currentPostIndex;

    exec("\"./IDL\" index.wpml > index.php");
    
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // verify user is logged in
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
            exec("\"./IDL\" stream.wpml > stream.php");
            exec("\"./IDL\" addAuthor.wpml > addAuthor.php");
        }

        // extract variables
        if (!empty($_POST["sortMode"])) {
            $sortMode = $_POST["sortMode"];
        }

        if (!isset($_POST["currentPostIndex"])) {
            $currentPostIndex = -1;
        }
        else {
            $currentPostIndex = $_POST["currentPostIndex"];
        }

        if (empty($_POST["currentStream"])) {
            $currentStream = "NO_STREAM";
    	}
    	else {
    	    $currentStream = $_POST["currentStream"];
        }

/*echo "$prevSortMode,$sortMode,$currentStream,$username,$stream";*/

        // perform operation if any
        if (isset($_POST["checkForNew"])) {
            unset($output);
