<?php include '../includes/header.php'; ?>
<?php include '../includes/db_connect.php'; ?>

<?php
$user_id = $_SESSION['user_id'];

$query = $conn->prepare("SELECT * FROM students WHERE student_id = ?");
$query->bind_param("i", $user_id);
$query->execute();
$student = $query->get_result()->fetch_assoc();
?>
<h1>Welcome, <?php echo $student['name']; ?></h1>
<p>CGPA: <?php echo $student['cgpa']; ?></p>
<p>Attendance: <?php echo $student['attendance']; ?>%</p>
<a href="../logout.php">Logout</a>
<?php include '../includes/footer.php'; ?>
