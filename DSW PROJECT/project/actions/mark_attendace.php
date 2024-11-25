<?php
session_start();
include '../includes/db_connect.php';

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $student_id = $_POST['student_id'];
    $status = $_POST['status'];

    $query = $conn->prepare("INSERT INTO attendance (student_id, date, status) VALUES (?, CURDATE(), ?)");
    $query->bind_param("is", $student_id, $status);
    if ($query->execute()) {
        $message = "Attendance marked successfully.";
    } else {
        $message = "Failed to mark attendance.";
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Mark Attendance</title>
    <link rel="stylesheet" href="../assets/css/styles.css">
</head>
<body>
    <form method="POST" action="">
        <h2>Mark Attendance</h2>
        <?php if (isset($message)) { echo "<p style='color: green;'>$message</p>"; } ?>
        <label for="student_id">Student ID:</label>
        <input type="text" name="student_id" required>
        <label for="status">Status:</label>
        <select name="status" required>
            <option value="Present">Present</option>
            <option value="Absent">Absent</option>
        </select>
        <button type="submit">Submit</button>
    </form>
    <a href="../dashboard/teacher.php">Back to Dashboard</a>
</body>
</html>
