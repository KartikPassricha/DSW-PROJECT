<?php
session_start();
include('db_connection.php');

$student_id = $_POST['student_id'];
$batch_id = $_POST['batch_id'];
$updateDate = date("Y-m-d");

// Check last modified date
$query = "SELECT modified_date FROM attendance WHERE student_id = $student_id AND batch_id = $batch_id ORDER BY modified_date DESC LIMIT 1";
$result = mysqli_query($db, $query);
$lastModified = mysqli_fetch_assoc($result)['modified_date'];

if ($lastModified && strtotime($updateDate) - strtotime($lastModified) <= 4 * 24 * 60 * 60) {
    $date = date("Y-m-d");
    $attendanceQuery = "SELECT * FROM attendance WHERE student_id = $student_id AND batch_id = $batch_id AND date = '$date'";
    $attendanceResult = mysqli_query($db, $attendanceQuery);

    if (mysqli_num_rows($attendanceResult) == 0) {
        $insertQuery = "INSERT INTO attendance (student_id, batch_id, date, modified_date) VALUES ($student_id, $batch_id, '$date', '$updateDate')";
        if (mysqli_query($db, $insertQuery)) {
            echo "Attendance marked successfully.";
        } else {
            echo "Error: " . mysqli_error($db);
        }
    } else {
        echo "Attendance has already been marked for today.";
    }
} else {
    echo "Updates allowed within 4 days only.";
}
?>
