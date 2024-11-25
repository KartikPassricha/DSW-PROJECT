<?php
session_start();
if (!isset($_SESSION['user_id'])) {
    header("C:/xampp/htdocs/DSW PROJECT/project/login.php");
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard</title>
    <link rel="stylesheet" href="C:/xampp/htdocs/DSW PROJECT/project/assets/css/styles.css">
</head>
<body>
