<?php
session_start();
include 'C:/xampp/htdocs/DSW PROJECT/project/includes/db_connect.php';

if ($_SESSION['role'] !== 'developer') {
    header("Location: login.php");
    exit();
}

if (isset($_POST['add_user'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];
    $role = $_POST['role'];

    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    $query = $conn->prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
    $query->bind_param("sss", $username, $hashed_password, $role);
    $query->execute();

    echo "User added successfully!";
}

if (isset($_POST['update_user'])) {
    $user_id = $_POST['user_id'];
    $username = $_POST['username'];
    $password = $_POST['password'];
    $role = $_POST['role'];

    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    $query = $conn->prepare("UPDATE users SET username = ?, password = ?, role = ? WHERE user_id = ?");
    $query->bind_param("sssi", $username, $hashed_password, $role, $user_id);
    $query->execute();

    echo "User updated successfully!";
}

if (isset($_GET['delete_user'])) {
    $user_id = $_GET['delete_user'];

    $query = $conn->prepare("DELETE FROM users WHERE user_id = ?");
    $query->bind_param("i", $user_id);
    $query->execute();

    echo "User deleted successfully!";
}

if (isset($_POST['update_cgpa'])) {
    $user_id = $_POST['user_id'];
    $cgpa = $_POST['cgpa'];

    $query = $conn->prepare("UPDATE students SET cgpa = ? WHERE user_id = ?");
    $query->bind_param("di", $cgpa, $user_id);
    $query->execute();

    echo "CGPA updated successfully!";
}

if (isset($_POST['update_timetable'])) {
    $user_id = $_POST['user_id'];
    $timetable = $_POST['timetable'];

    $query = $conn->prepare("UPDATE timetables SET timetable = ? WHERE user_id = ?");
    $query->bind_param("si", $timetable, $user_id);
    $query->execute();

    echo "Timetable updated successfully!";
}

$search_query = "";
if (isset($_POST['search_id'])) {
    $search_id = $_POST['search_id'];
    $search_query = " WHERE user_id = '$search_id'";
}

$roles = ['student', 'teacher', 'developer'];
$user_data = [];

foreach ($roles as $role) {
    $query = $conn->prepare("SELECT * FROM users WHERE role = ? $search_query");
    $query->bind_param("s", $role);
    $query->execute();
    $result = $query->get_result();
    $user_data[$role] = $result->fetch_all(MYSQLI_ASSOC);
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Developer Dashboard - User Management</title>
    <link rel="stylesheet" href="assets/css/style1.css">
</head>
<body>
    <div name="abc">
    <h1>Welcome, Developer</h1>

    <h2>Add New User</h2>
    <form method="POST">
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

    <hr>

    <!-- Search for User by ID -->
    <h2>Search User by ID</h2>
    <form method="POST">
        <label for="search_id">Enter User ID:</label>
        <input type="text" name="search_id" placeholder="Search by ID">
        <button type="submit">Search</button>
    </form>
    
    <hr>

    <?php foreach ($user_data as $role => $users): ?>
        <h2>Manage <?php echo ucfirst($role); ?>s</h2>
        <table border="1">
            <thead>
                <tr>
                    <th>Username</th>
                    <th>Role</th>
                    <th>Actions</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($users as $user): ?>
                    <tr>
                        <td><?php echo $user['username']; ?></td>
                        <td><?php echo $user['role']; ?></td>
                        <td>
                            <!-- Update User Form -->
                            <form method="POST" style="display:inline;">
                                <input type="hidden" name="user_id" value="<?php echo $user['user_id']; ?>">
                                <input type="text" name="username" value="<?php echo $user['username']; ?>" required>
                                <input type="password" name="password" placeholder="New password" required>
                                <select name="role" required>
                                    <option value="student" <?php if ($user['role'] == 'student') echo 'selected'; ?>>Student</option>
                                    <option value="teacher" <?php if ($user['role'] == 'teacher') echo 'selected'; ?>>Teacher</option>
                                    <option value="developer" <?php if ($user['role'] == 'developer') echo 'selected'; ?>>Developer</option>
                                </select>
                                <button type="submit" name="update_user">Update</button>
                            </form>

                            <!-- Update CGPA (only for students) -->
                            <?php if ($user['role'] == 'student'): ?>
                                <form method="POST" style="display:inline;">
                                    <input type="hidden" name="user_id" value="<?php echo $user['user_id']; ?>">
                                    <input type="number" name="cgpa" placeholder="Enter CGPA" step="0.01" required>
                                    <button type="submit" name="update_cgpa">Update CGPA</button>
                                </form>
                            <?php endif; ?>

                            <!-- Update Timetable (for students and teachers) -->
                            <?php if (in_array($user['role'], ['student', 'teacher'])): ?>
                                <form method="POST" style="display:inline;">
                                    <input type="hidden" name="user_id" value="<?php echo $user['user_id']; ?>">
                                    <textarea name="timetable" placeholder="Enter Timetable" required></textarea>
                                    <button type="submit" name="update_timetable">Update Timetable</button>
                                </form>
                            <?php endif; ?>

                            <!-- Delete User -->
                            <a href="developer.php?delete_user=<?php echo $user['user_id']; ?>" onclick="return confirm('Are you sure you want to delete this user?')">Delete</a>
                        </td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
        <br>
    <?php endforeach; ?>
    </div>
</body>
</html>

