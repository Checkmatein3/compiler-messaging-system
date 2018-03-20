<?php
    exec("\"./IDL\" index.wpml > index.php");
    exec("\"./IDL\" stream.wpml > stream.php");
    exec("\"./IDL\" addAuthor.wpml > addAuthor.php");
?>