<html>
<body>
<H3>Message Viewer Login</H3>

<?php
    exec("\"./IDL\" index.wpml > index.php");
    exec("\"./IDL\" stream.wpml > stream.php");
    exec("\"./IDL\" addAuthor.wpml > addAuthor.php");
?>
<form method="POST" action="stream.php" >
<p>Username: <input name="username" value=""></p>
<p><input type="submit" name="submit" value="Login"></p>
</form>

<form method="POST" action="addAuthor.php">
<input type="submit" name="indexAddAuthor" value="Add/Remove Author"><br>
</form>

</html>
</body>
