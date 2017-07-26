#pragma once

class Spammer
{
public:

	void spam();
};

extern std::unique_ptr<Spammer> spammer;