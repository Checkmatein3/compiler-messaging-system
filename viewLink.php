<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if(empty($loginError)) {
        echo <<<_FORM
<form method="POST" action="view.php" >
<p><input name="username" type="hidden" value="$username"></p>
<p><input name="stream" type="hidden" value="$stream"></p>
<p><input type="submit" name="checkForNew" value="Back to View"></p>
</form>
_FORM;
        exec("\"./IDL\" view.wpml > view.php");
    }
}
?>