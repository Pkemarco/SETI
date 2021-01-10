void PIDf() {
// Get the position of the line.
	current_pos_of_line = m3pi.line_position();
	proportional = current_pos_of_line;
// Compute the derivative
	derivative = current_pos_of_line - previous_pos_of_line;
// Compute the integral
	integral = (integral + I_TERMO * proportional) / (1 + I_TERMO);
// Remember the last position.
	previous_pos_of_line = current_pos_of_line;
// Compute the power
	power = (proportional * (P TERM)) + (integral * (I_TERM)) + (derivative * (D_TERM)) ;
// Compute new speeds
	right = speed - (power * MAX);
	left = speed + (power * MAX);
// limit checks on motor control
	// MIN < right < MAX
	if (right < MIN)
		right = MIN;
	else if (right > MAX)
		right = MAX;
	// MIN < left < MAX
	if (left < MIN)
		left = MIN;
	else if (left > MAX)
		left = MAX;
// send command to motors
	m3pi.left_motor(left);
	m3pi.right_motor(right);
}

void PIDi() {
// nouvelle position de la ligne
	current_pos_of_linei = 1000 * m3pi.line_position();
	proportionali = current_pos_of_linei;
// Compute the derivative
	derivativei = current_pos_of_linei - previous_pos_of_linei;
// Compute the integral
	integrali = (integrali + I_TERMOI * proportionali) / (1 + I_TERMOI);
// Remember the last position.
	previous_pos_of_linei = current_pos_of_linei;
// Compute the power
	poweri = (proportionali * (P TERMI)) + (integrali * (I_TERMI)) + (derivativei * (D_TERMI)) ;
// Compute new speeds
	righti = speedi - (poweri * MAXI) / 1000;
	lefti = speedi + (poweri * MAXI) / 1000;
// limit checks on motor control
	// MIN < righti < MAX
	if (righti < MIN)
		righti = MIN;
	else if (righti > MAX)
		righti = MAX;
	// MIN < lefti < MAX
	if (lefti < MIN)
		lefti = MIN;
	else if (lefti > MAX)
		lefti = MAX;
// Control motors
	m3pi.left_motor(lefti / 1000.);
	m3pi.right_motor(righti / 1000.);
}