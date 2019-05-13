<?php
require "csvtools.php";
function checkLogin() {
    $userscsv = "../users.csv";
    if (!isset($_COOKIE["username"]) or !isset($_COOKIE["password"])) {
        return null;
    }
    $username = $_COOKIE["username"];
    $password = $_COOKIE["password"];
    $users = content($userscsv);
    foreach ($users as $user) {
        if ($user[0] == "$username" && $user[1] == "$password") {
            return $username;
        }
    }
    return null;
}

function login($username, $password) {
    $userscsv = "../users.csv";
    $users = content($userscsv);
    foreach ($users as $user) {
        if ($user[0] == "$username" && $user[1] == "$password") {
            return $username;
        }
    }
    return null;
}
?>