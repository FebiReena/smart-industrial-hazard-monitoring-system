const tempText =
document.getElementById("temperature");

const distanceText =
document.getElementById("distance");

const intrusionText =
document.getElementById("intrusion");

const statusText =
document.getElementById("status");

const alertsList =
document.getElementById("alerts");

const clock =
document.getElementById("clock");

let labels = [];
let tempHistory = [];

const ctx =
document.getElementById("tempChart");

const tempChart =
new Chart(ctx, {

type:"line",

data:{

labels:labels,

datasets:[{

data:tempHistory,

borderWidth:2,

tension:0.4

}]
},

options:{

responsive:true,

maintainAspectRatio:false,

animation:false,

plugins:{
legend:{
display:false
}
}
}
});

async function updateDashboard()
{
    try{

        const response =
        await fetch("/data");

        const data =
        await response.json();

        tempText.innerText =
        data.temperature + " °C";

        distanceText.innerText =
        data.distance + " cm";

        intrusionText.innerText =
        data.intrusion
        ? "DETECTED"
        : "SAFE";

        statusText.innerText =
        data.status;

        const now =
        new Date();

        const label =
        now.toLocaleTimeString([],{
        hour:'2-digit',
        minute:'2-digit'
        });

        labels.push(label);

        tempHistory.push(
        data.temperature
        );

        if(labels.length > 15)
        {
            labels.shift();
            tempHistory.shift();
        }

        tempChart.update();

        if(data.status !== "NORMAL")
        {
            if(
            alertsList.children.length===1 &&
            alertsList.children[0].innerText==="No alerts yet"
            )
            {
                alertsList.innerHTML="";
            }

            const li =
            document.createElement("li");

            li.innerText =
            `${label} - ${data.status}`;

            alertsList.prepend(li);

            while(alertsList.children.length > 5)
            {
                alertsList.removeChild(
                alertsList.lastChild
                );
            }
        }

    }catch(error){

        console.log(error);
    }
}

setInterval(()=>{

clock.innerText =
new Date().toLocaleTimeString([],{
hour:'2-digit',
minute:'2-digit'
});

},1000);

updateDashboard();

setInterval(
updateDashboard,
5000
);