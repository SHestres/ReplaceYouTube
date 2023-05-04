'use strict';
var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/', (req, res) => {
    res.send('User list page');
});


router.post("/", (req, res) => {
    res.send("Creating new user");
})

router.get("/new", (req, res) => {
    res.send("New User Form");
});


router.route("/:id")
    .get((req, res) => {
        res.send(`You're viewing user ${req.params.id}`);
    })
    .put((req, res) => {
        res.send(`You're updating user ${req.params.id}`);
    })
    .delete((req, res) => {
        res.send(`You're deleting user ${req.params.id}`);
    })


module.exports = router;
