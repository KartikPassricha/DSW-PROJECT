<?php
session_start();
include 'includes/db_connect.php';

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $username = $_POST['username'];
    $password = $_POST['password'];
    $role = $_POST['role'];

    // Debugging: Print out form data
    var_dump($_POST);  // Check if the form is sending the right data
    echo "<br>";

    // Prepare query to validate login credentials and role
    $query = $conn->prepare("SELECT * FROM users WHERE username = ? AND role = ?");
    $query->bind_param("ss", $username, $role);
    $query->execute();
    $result = $query->get_result();

    // Debugging: Check if a user was found
    if ($result->num_rows > 0) {
        echo "User found!";  // Check if a user is returned
        $user = $result->fetch_assoc();
        var_dump($user); // Print out user details
        echo "<br>";

        if (password_verify($password, $user['password'])) {
            // Password matches, proceed to login
            $_SESSION['user_id'] = $user['user_id'];
            $_SESSION['role'] = $user['role'];
            
            // Redirect based on role
            if ($user['role'] == 'student') {
                header("Location: dashboard/student.php");
            } elseif ($user['role'] == 'teacher') {
                header("Location: dashboard/teacher.php");
            } elseif ($user['role'] == 'developer') {
                header("Location: dashboard/developer.php");
            }
            exit();
        } else {
            // Invalid password
            echo "Incorrect password.";
        }
    } else {
        // No matching user found
        echo "No matching user found.";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <title>Login</title>
    <link rel="stylesheet" href="assets/css/styles.css">
</head>
<body>
    <form method="POST" action="">
        <h2>Login</h2>
        <?php if (isset($error)) { echo "<p style='color:red;'>$error</p>"; } ?>
        <label for="username">Username:</label>
        <input type="text" name="username" placeholder="Enter username" required>

        <label for="password">Password:</label>
        <input type="password" name="password" placeholder="Enter password" required>

        <label for="role">Role:</label>
        <select name="role" required>
            <option value="student">Student</option>
            <option value="teacher">Teacher</option>
            <option value="developer">Developer</option>
        </select>

        <button type="submit">Login</button>
    </form>
</body>
</html>
