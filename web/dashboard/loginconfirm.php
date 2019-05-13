<?php
require "../usertools.php";
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST["username"]) and isset($_POST["password"])) {
		$password = $_POST["password"];
		$username = $_POST["username"];
		if (login($username, $password) != null) {
		    setcookie("username", $username);
		    setcookie("password", $password);
			echo "ok";
		} 
		else {
			echo "incorrect user name or password";
		}
	}
}
exit();
?>