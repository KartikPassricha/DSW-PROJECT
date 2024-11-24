<?php
session_start();
include('db_connection.php');

$username = $_POST['username'];
$password = $_POST['password'];
$role = $_POST['role'];

// Authentication logic (simplified for example)
if ($role == 'teacher') {
    $_SESSION['role'] = 'teacher';
    header('Location: teacher_dashboard.php');
} elseif ($role == 'student') {
    $_SESSION['role'] = 'student';
    $_SESSION['student_id'] = 1; // Assuming student ID is 1 for testing
    header('Location: student_dashboard.php');
} else {
    echo "Invalid login credentials";
}
?>
