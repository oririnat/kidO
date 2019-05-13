<?php
    require "../usertools.php";
    if (checkLogin() != null) {
        header("Location: see.php");
        exit();
    }
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<title>KidO login</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">

	<script src="hashing.js"></script>
	<style>
	#error {
	color: red;
	}
	</style>

<!--===============================================================================================-->
	<link rel="icon" type="image/png" href="images/icons/favicon.ico"/>
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/bootstrap/css/bootstrap.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/font-awesome-4.7.0/css/font-awesome.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/iconic/css/material-design-iconic-font.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animate/animate.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/css-hamburgers/hamburgers.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animsition/css/animsition.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/select2/select2.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/daterangepicker/daterangepicker.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="css/util.css">
	<link rel="stylesheet" type="text/css" href="css/main.css">
<!--===============================================================================================-->
</head>

<body>
	<div class="container-login100" style="background-image: url('images/bg-01.jpg');">
		<div class="wrap-login100 p-l-55 p-r-55 p-t-80 p-b-30">
			<form class="login100-form validate-form">
				<span class="login100-form-title p-b-37">
					kidO - Sign In
				</span>

				<div class="wrap-input100 validate-input m-b-20" data-validate="try again">
					<input class="input100" type="text" placeholder="username" id="username">
					<span class="focus-input100"></span>
				</div>
				<br>
				<div class="wrap-input100 validate-input m-b-20" data-validate="try again">
					<input class="input100" type="password" placeholder="password" id="password">
					<span class="focus-input100"></span>
				</div>
				<p align="center" id="error"></p>
				<br>				
				
				<div class="container-login100-form-btn">
					<button onclick = "login()"> Login </button>
				</div>
				
				<script>
					function login() {
						var formData = new FormData();
						formData.append("username", document.getElementById("username").value);
						formData.append("password",MD5(document.getElementById("password").value));
						var xhr = new XMLHttpRequest();
						xhr.open("POST", "loginconfirm.php");
						xhr.send(formData);
						xhr.onreadystatechange = function() {
							if (this.readyState == 4){
								if (this.status == 200) { // allways 0
								    var data = xhr.responseText;
									if (data === "ok") {
										document.cookie = "username="+document.getElementById("username").value;
										document.cookie = "password="+MD5(document.getElementById("password").value);
										open("see.php","_self");
									} else {
										document.getElementById("error").innerHTML = data;
									}
								}
							}
						};
					}
				</script>

				<div class="text-center p-t-57 p-b-20">
					<span class="txt1">
						Or login with
					</span>
				</div>

				<div class="flex-c p-b-112">
					<a href="#" class="login100-social-item">
						<i class="fa fa-facebook-f"></i>
					</a>

					<a href="#" class="login100-social-item">
						<img src="images/icons/icon-google.png" alt="GOOGLE">
					</a>
				</div>

				<div class="text-center">
					<span class="txt1">
						Sign up in the kidO APP
					</span>
				</div>
			</form>
		</div>
	</div>

	<div id="dropDownSelect1"></div>

<!--===============================================================================================-->
	<script src="vendor/jquery/jquery-3.2.1.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/animsition/js/animsition.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/bootstrap/js/popper.js"></script>
	<script src="vendor/bootstrap/js/bootstrap.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/select2/select2.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/daterangepicker/moment.min.js"></script>
	<script src="vendor/daterangepicker/daterangepicker.js"></script>
<!--===============================================================================================-->
	<script src="vendor/countdowntime/countdowntime.js"></script>
<!--===============================================================================================-->
	<script src="js/main.js"></script>

</body>
</html>
