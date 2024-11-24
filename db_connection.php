<?php
$host = 'localhost';
$username = 'root';
$password = '#LOWSPECGAMER1234@';
$database = 'login_system';

$db = new mysqli($host, $username, $password, $database);

if ($db->connect_error) {
    die("Connection failed: " . $db->connect_error);
}
?>
