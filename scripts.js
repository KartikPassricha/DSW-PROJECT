document.getElementById("loginForm").addEventListener("submit", function(event) {
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const role = document.getElementById("role").value;

    if (!username || !password || !role) {
        event.preventDefault();
        document.getElementById("errorMessage").textContent = "All fields are required!";
    }
});
