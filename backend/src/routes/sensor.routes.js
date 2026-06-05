import { Router } from "express";
import { getLatestSensorData } from "../controller/sensor.controller.js";

const router = Router();

router.get("/latest", getLatestSensorData);

export default router;