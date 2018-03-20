<html>
<body>
<H3>View</H3>

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

        if (empty($_POST["currentPostIndex"]) && $_POST["currentPostIndex"] != 0) {
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

$output = [];
//run with py for windows, python3 for unix
exec("python3 webview.py printNextPost $sortMode -1 NO_STREAM+$username+$stream", $output, $status);

	       $messageToDisplay = extractPost($output, $currentStream, $currentPostIndex);
        }
        else if (isset($_POST["previous"])) {
            unset($output);

$output = [];
//run with py for windows, python3 for unix
exec("python3 webview.py printPrevPost $sortMode $currentPostIndex $currentStream+$username+$stream", $output, $status);

	       $messageToDisplay = extractPost($output, $currentStream, $currentPostIndex);
        }
        else if (isset($_POST["next"])) {
            unset($output);

$output = [];
//run with py for windows, python3 for unix
exec("python3 webview.py printNextPost $sortMode $currentPostIndex $currentStream+$username+$stream", $output, $status);

	       $messageToDisplay = extractPost($output, $currentStream, $currentPostIndex);
        }
        else {
            if (isset($_POST["toggleSort"])) {
                if($sortMode == 'sortByUsername') {
                    $sortMode = 'sortByDate';
                }
                else if($sortMode == 'sortByDate') {
                    $sortMode = 'sortByUsername';
                }
            }
            else if (isset($_POST["markAll"])) {
                unset($output);

$output = [];
//run with py for windows, python3 for unix
exec("python3 webview.py markAllPostsAsRead $stream+$username", $output, $status);

                if(!$status) {
                    echo "All posts for current streams marked as read.";
                }
                else {
                    echo "Unable to mark all posts for current streams as read.";
                }
            }

            unset($output);

$output = [];
//run with py for windows, python3 for unix
exec("python3 webview.py printCurrentPost $sortMode $currentPostIndex $currentStream+$username+$stream", $output, $status);

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

<form method="POST" action="post.php" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="postToStream" value="Post to Stream"></p>
</form>

<form method="POST" action="stream.php" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input type="submit" name="changeStream" value="Change Stream"></p>
</form>

<form method="POST" action="addAuthor.php" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="addAuthor" value="Add/Remove Author"></p>
</form>

<HR>

<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="currentPostIndex" type="hidden" value="<?php echo $currentPostIndex;?>"></p>
<p><input name="currentStream" type="hidden" value="<?php echo $currentStream;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="markAll" value="Mark All Posts As Read"></p>
</form>

<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="currentPostIndex" type="hidden" value="<?php echo $currentPostIndex;?>"></p>
<p><input name="currentStream" type="hidden" value="<?php echo $currentStream;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="checkForNew" value="Check For New"></p>
</form>

<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="currentPostIndex" type="hidden" value="<?php echo $currentPostIndex;?>"></p>
<p><input name="currentStream" type="hidden" value="<?php echo $currentStream;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="toggleSort" value="<?php if($sortMode == 'sortByUsername') {echo 'Sort By Date';} else if($sortMode == 'sortByDate') {echo 'Sort By Username';} ?>"></p>
</form>

<HR>

<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="currentPostIndex" type="hidden" value="<?php echo $currentPostIndex;?>"></p>
<p><input name="currentStream" type="hidden" value="<?php echo $currentStream;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="previous" value="Previous"></p>
</form>

<form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>" >
<p><input name="sortMode" type="hidden" value="<?php echo $sortMode;?>"></p>
<p><input name="currentPostIndex" type="hidden" value="<?php echo $currentPostIndex;?>"></p>
<p><input name="currentStream" type="hidden" value="<?php echo $currentStream;?>"></p>
<p><input name="username" type="hidden" value="<?php echo $username;?>"></p>
<p><input name="stream" type="hidden" value="<?php echo $stream;?>"></p>
<p><input type="submit" name="next" value="Next"></p>
</form>

<HR>

<?php
    echo "<p>";
    echo "$messageToDisplay";
    echo "</p>";
?>

<HR>

<a href="index.php">Change User</a><br>

</html>
</body>
