<?php
session_start();
include '../includes/db_connect.php';

if ($_SESSION['role'] != 'developer') {
    header("Location: ../login.php");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST['add_user'])) {
        $username = $_POST['username'];
        $password = password_hash($_POST['password'], PASSWORD_BCRYPT);
        $role = $_POST['role'];

        $query = $conn->prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
        $query->bind_param("sss", $username, $password, $role);

        if ($query->execute()) {
            $message = "User added successfully.";
        } else {
            $message = "Failed to add user.";
        }
    }

    if (isset($_POST['delete_user'])) {
        $user_id = $_POST['user_id'];

        $query = $conn->prepare("DELETE FROM users WHERE user_id = ?");
        $query->bind_param("i", $user_id);

        if ($query->execute()) {
            $message = "User deleted successfully.";
        } else {
            $message = "Failed to delete user.";
        }
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Manage Users</title>
    <link rel="stylesheet" href="../assets/css/styles.css">
</head>
<body>
    <h2>Manage Users</h2>
    <?php if (isset($message)) { echo "<p style='color: green;'>$message</p>"; } ?>
    <form method="POST" action="">
        <h3>Add User</h3>
        <label for="username">Username:</label>
        <input type="text" name="username" required>
        <label for="password">Password:</label>
        <input type="password" name="password" required>
        <label for="role">Role:</label>
        <select name="role" required>
            <option value="student">Student</option>
            <option value="teacher">Teacher</option>
            <option value="developer">Developer</option>
        </select>
        <button type="submit" name="add_user">Add User</button>
    </form>

    <form method="POST" action="">
        <h3>Delete User</h3>
        <label for="user_id">User ID:</label>
        <input type="text" name="user_id" required>
        <button type="submit" name="delete_user">Delete User</button>
    </form>
    <a href="../dashboard/developer.php">Back to Dashboard</a>
</body>
</html>
