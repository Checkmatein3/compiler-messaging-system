        if (count($output) == 0) {
            echo "<a href=\"index.php\">Back to Login</a><br>";
            die("The user does not have access to any streams.");
        }
        else {
            foreach($output as $stream) {
                echo "$stream<br>";
            }
        }
    }
    else {
        echo "<a href=\"index.php\">Back to Login</a><br>";
        die("No user logged in.");
    }
?>
