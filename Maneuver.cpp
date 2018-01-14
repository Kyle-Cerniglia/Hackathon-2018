
void avoidObject(int objClass, double startAngle) {
	double radius;
	double dist;
	double angle;
	if (objClass == 1) { radius = 3; }
	else { radius = 1; };
	// turn right 63 deg
	while (angle < 90) {
		while (dist >= radius + 0.2 || dist <= radius - 0.2) {
			// update
			// forward
		}
		if (dist < radius) { /* turn 5 deg right */ }
		else { /* turn 5 deg left */ };
	}
}