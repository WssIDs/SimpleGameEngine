#include "Level.h"
#include <sstream>
#include "boost\archive\binary_oarchive.hpp"
#include "boost\archive\binary_iarchive.hpp"

void Level::Init(const std::string& filename)
{
	//Bind::Texture tex;
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	boost::archive::binary_oarchive oa(ofs, boost::archive::no_header);
	oa << (*this);
}

void Level::Load(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);
	boost::archive::binary_iarchive ia(ifs, boost::archive::no_header);
	ia >> (*this);
}
