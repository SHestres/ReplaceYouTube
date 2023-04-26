const express = require("express");
const cors = require("cors");

const app = express();


app.set("view engine", "ejs");

//app.use(express.static("public"));
app.use(express.json());

var corsOptions = {
    origin: 'http://localhost:5173',
    optionsSuccessStatus: 200
}

app.use(cors(corsOptions));

app.get("/", (req, res) => {
    res.send("Hi. This is the base page");
});

app.post('/api/login', (req, res) =>{
    let users = require("./assets/users.json")
    console.log(users)
    let userByName = users[req.body.username];
    console.log(userByName);
    let user;
    if(userByName){
        if(userByName.password == req.body.password){
            user = userByName.name;
        }
    }
    console.log(user);
    res.json({user});
})


const usersRouter = require("./routes/users.js");

app.use("/users", usersRouter);


app.listen("5100");
