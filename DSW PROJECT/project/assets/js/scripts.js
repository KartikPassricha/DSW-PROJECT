document.addEventListener("DOMContentLoaded", () => {
    const messageBox = document.querySelector("#messageBox");

    if (messageBox) {
        setTimeout(() => {
            messageBox.style.display = "none";
        }, 3000); 
    }
});
