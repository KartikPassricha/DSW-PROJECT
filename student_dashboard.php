<?php
session_start();
include('db_connection.php');

$student_id = $_SESSION['student_id'];
$batch_id = $_SESSION['batch_id'];

$totalDaysQuery = "SELECT COUNT(DISTINCT date) as total_days FROM attendance WHERE batch_id = $batch_id";
$totalDaysResult = mysqli_query($db, $totalDaysQuery);
$totalDays = mysqli_fetch_assoc($totalDaysResult)['total_days'];

$attendedDaysQuery = "SELECT COUNT(date) as attended_days FROM attendance WHERE student_id = $student_id AND batch_id = $batch_id";
$attendedDaysResult = mysqli_query($db, $attendedDaysQuery);
$attendedDays = mysqli_fetch_assoc($attendedDaysResult)['attended_days'];

$attendancePercentage = $totalDays > 0 ? ($attendedDays / $totalDays) * 100 : 0;

echo "<p>Attendance Percentage: " . round($attendancePercentage, 2) . "%</p>";
?>
