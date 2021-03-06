const config = require("../config/config.json");
const { Sequelize, Op } = require("sequelize");
const db = {};

const sequelize = new Sequelize(config.database, config.username, config.password, {
  host: config.host,
  dialect: config.dialect,
});

sequelize
  .authenticate()
  .then((err) => console.log("\n~ Connected to DataBase " + config.database))
  .catch((err) => console.log("\n~ Unable to connect to db" + err));

db.Sequelize = Sequelize;
db.sequelize = sequelize;
db.Op = Op;

db.Visitors = require("./Visitor.model.js")(sequelize, Sequelize);
db.Views = require("./Views.model.js")(sequelize, Sequelize);

module.exports = db;
