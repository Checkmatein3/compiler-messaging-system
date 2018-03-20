<html>
<body>
<form method="POST" action="page">
<input type="submit" name="Description" btnText="Submit"><br>

<H5>some header!</H5>

<HR>

<?php
	$exePath = $exe = 'ls';
	if (!file_exists($exe)) {
	$exePath = "./bin/".$exe;
		if (!file_exists($exePath)) {
			$exePath = "/bin/".$exe;
		}
	}

	exec($exePath, $output, $status);
	if ($status) {
		echo "exec failed";
	}
	else {
		foreach ($output as $line) {
			echo "$line<br>";
		}
	}
?>

<form method="POST" action="filename" >
this line has 3 elements<input name="some name" value="some value"><br>
<input type="submit" value="Submit"><br>
</form>

<pre >
testHTML<BR>
with multiple lines

</pre>

<pre >
empty line in file above this element
</pre>

<form method="POST" action="filename" >
<input name="some name" value="some value"checked alt="hi" ><br>
<input name="some name" value="a second value"><br>
<input name="some name" value="a third value"><br>
<input type="submit" value="Submit"><br>
</form>

<pre >
Default text
</pre>

<form method="POST" action="phone.php" >
Phone Number<input name="number" value="123-456-7890"><br>
Name<input name="name" value="Bob"><br>
<input type="submit" value="Submit"><br>
</form>

<pre >
the following elements will use default attributes
</pre>

<H3>HEADING</H3>

<a href="url">link</a><br>

<img src="some image" style="width:100;height:100;"><br>

<pre >
Default text
</pre>

</html>
</body>
