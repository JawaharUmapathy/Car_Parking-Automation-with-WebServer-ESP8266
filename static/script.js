let currentCount = 0;

async function fetchCount() {
    try {
        const res = await fetch("/get_count");
        const data = await res.json();
        updateSlots(data.count);
    } catch (err) {
        console.error("Error fetching count:", err);
    }
}

function updateSlots(newCount) {
    document.getElementById("total").textContent = newCount;
    document.getElementById("full-indicator").style.display = newCount >= 8 ? "inline" : "none";

    for (let i = 1; i <= 8; i++) {
        let car = document.querySelector(`#slot-${i} .car`);
        let buyBtn = document.querySelector(`#slot-${i} .buy-btn`);

        if (i <= newCount) {
            // Occupied → animate car in
            car.style.display = "block";
            setTimeout(() => car.classList.add("show"), 50);
            car.classList.remove("hide");
            buyBtn.style.display = "none";
        } else {
            // Empty → animate car out
            if (car.classList.contains("show")) {
                car.classList.add("hide");
                setTimeout(() => {
                    car.classList.remove("show");
                    car.style.display = "none";
                }, 600); // wait for animation
            }
            buyBtn.style.display = "inline-block";
        }
    }

    currentCount = newCount;
}

// Refresh every 2s
setInterval(fetchCount, 2000);

// Initial load
fetchCount();
