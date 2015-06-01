#include <odb\core.hxx>

//use db object to tell the compiler that the class that follows is persistent
#pragma db object
class questions
{
public:
	questions(unsigned int number,
		const std::string& text,
		const std::string& variable,
		const std::string& directory);

	const std::string& text() const;
	const std::string& variable() const;
	const std::string& directory() const;

	unsigned int number() const;
	void number(unsigned int);

private:

	//defult constructor
	questions() {}

	//make odb::access a friend class. This makes the defult constructor and the data...
	//...members accessible tot he database support code
	friend class odb::access;

	//db id auto tells compiler that the following member is the object's identifier.
#pragma db id auto
	//Give unique identifier to each data member
	unsigned long id_;


	std::string text_;
	std::string variable_;
	std::string directory_;

	unsigned int number_;
};