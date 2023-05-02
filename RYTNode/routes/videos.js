var express = require('express');

var router = express.Router();

router.use(express.static('assets/videos'))

module.exports = router;