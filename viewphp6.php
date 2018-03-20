            $messageToDisplay = extractPost($output, $currentStream, $currentPostIndex);
        }
    }
    else {
        echo "<a href=\"index.php\">Back to Login</a><br>";
        die("No user logged in.");
    }

    function extractPost($output, &$currentStream, &$currentPostIndex) {
    /*foreach($output as $line) {
        echo "$line<br>";
    }*/

        $currentStream = $output[0];
        $currentPostIndex = $output[1];
/*echo "<br>$currentPostIndex,$currentStream";*/
        $output = array_slice($output, 2);
        $messageToDisplay = implode("<br>", $output);
        return $messageToDisplay;
    }
?>
