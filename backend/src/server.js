import express from "express";
import cors from "cors";
import dotenv from "dotenv";
import sensorRoutes from "./routes/sensor.routes.js"


dotenv.config();
const app = express();
app.use(cors());
app.use(express.json());



app.get ("/",(req , res)=>{
    res.send("Backend is alive");
});

app.use("/api/sensor", sensorRoutes);

const PORT = process.env.PORT || 5000;
app.listen(PORT,()=>{
    console.log(`The server is running on: http://localhost:${PORT}`);
}) ;