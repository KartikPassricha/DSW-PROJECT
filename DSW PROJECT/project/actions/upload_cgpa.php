<?php
session_start();
include '../includes/db_connect.php';

if ($_SESSION['role'] != 'teacher') {
    header("Location: ../login.php");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $student_id = $_POST['student_id'];
    $cgpa = $_POST['cgpa'];

    $query = $conn->prepare("UPDATE students SET cgpa = ? WHERE student_id = ?");
    $query->bind_param("di", $cgpa, $student_id);
    if ($query->execute()) {
        $message = "CGPA updated successfully.";
    } else {
        $message = "Failed to update CGPA.";
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Upload CGPA</title>
    <link rel="stylesheet" href="../assets/css/styles.css">
</head>
<body>
    <form method="POST" action="">
        <h2>Upload CGPA</h2>
        <?php if (isset($message)) { echo "<p style='color: green;'>$message</p>"; } ?>
        <label for="student_id">Student ID:</label>
        <input type="text" name="student_id" required>
        <label for="cgpa">CGPA:</label>
        <input type="number" step="0.01" name="cgpa" required>
        <button type="submit">Submit</button>
    </form>
    <a href="../dashboard/teacher.php">Back to Dashboard</a>
</body>
</html>
