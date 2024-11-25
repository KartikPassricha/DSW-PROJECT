<?php

include 'includes/db_connect.php';

$query = "SELECT user_id, password FROM users";
$result = $conn->query($query);

if ($result->num_rows > 0) {
    while ($user = $result->fetch_assoc()) {

        $user_id = $user['user_id'];
        $password = $user['password'];

        if (strlen($password) !== 60) {  

            $hashed_password = password_hash($password, PASSWORD_DEFAULT);

            $update_query = $conn->prepare("UPDATE users SET password = ? WHERE user_id = ?");
            $update_query->bind_param("si", $hashed_password, $user_id);
            $update_query->execute();

            echo "Password for user ID $user_id updated successfully.<br>";
        } else {
            echo "User ID $user_id already has a hashed password.<br>";
        }
    }
} else {
    echo "No users found.";
}

?>
