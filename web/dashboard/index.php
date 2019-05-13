<?php
    require "../usertools.php";
    if (checkLogin() != null) {
        header("Location: see.php");
        exit();
    }
?>
<html>
    <body>
        <script>
            location.href = "login.php";
        </script>
    </body>
</html>
