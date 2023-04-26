const express = require("express");

const app = express();

app.set("view engine", "ejs");

//app.use(express.static("public"));
app.use(express.json());

app.get("/", (req, res) => {
    res.send("Hi. This is the base page");
});

app.post('/api/login', (req, res) =>{
    console.log('Trying to post');
    console.log(req.body);
    res.json({user: 'Mike'});
})

/*
app.post("/api/login", (req, res) =>{
    console.log('Trying to post');
    console.log(req.body);
    res.json({user: 'Mike'});
})
*/
const usersRouter = require("./routes/users.js");

app.use("/users", usersRouter);


app.listen("5100");
