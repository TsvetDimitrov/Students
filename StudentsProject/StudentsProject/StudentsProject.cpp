#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <iterator>
#include <sstream>
#include <list>
#include <algorithm>
#include <mysql.h>


class Student {
private:
	std::string studentName;
	int studentNumber;
	std::vector < int > studentGrades;
	double averageGrade = AverageGrade();

public:
	Student();
	Student(std::string studentName, int studentNumber, std::vector < int > studentGrades);
	double AverageGrade(void);
	void addNewGrade(int grade);
	void printAllInfo();
	std::string getName() { return studentName; };
	int getId() { return studentNumber; };
	std::vector < int > getStudentGrades() { return studentGrades;  };
	void showGrades() {
		for (int grade : studentGrades) {
			std::cout << grade << " ";
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const Student& student);


};


Student::Student() {
	this->studentName = "";
	this->studentNumber = 0;
}

Student::Student(std::string studentName, int studentNumber, std::vector < int > studentGrades) {
	this->studentName = studentName;
	this->studentNumber = studentNumber;
	this->studentGrades = studentGrades;
	double averageGrade = AverageGrade();
};

double Student::AverageGrade(void) {

	double average = accumulate(this->studentGrades.begin(), this->studentGrades.end(), 0.0) / this->studentGrades.size();
	return round(average);
}

void Student::addNewGrade(int grade) {
 	this->studentGrades.push_back(grade);
}
void Student::printAllInfo() {
	std::ostringstream oss;
	std::copy(this->studentGrades.begin(), this->studentGrades.end() - 1,
		std::ostream_iterator<int>(oss, ", "));
	oss << this->studentGrades.back();

	std::cout << "Name: " << this->studentName << " Faculty number: " << this->studentNumber<< " Average grade: " << this->AverageGrade() <<" Student grades: " << oss.str() <<std::endl;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
	os << "StudentID: " << student.studentNumber << " Name: " << student.studentName << " Average Grade: ";
	int avGr;
	int totalSum = 0;
		for (std::vector<int>::const_iterator i = student.studentGrades.begin(); i != student.studentGrades.end(); ++i) {
			totalSum += *i;
		}
		avGr = totalSum / student.studentGrades.size();
		os << avGr << " All grades: ";

		for (std::vector<int>::const_iterator i = student.studentGrades.begin(); i != student.studentGrades.end(); ++i) {
			os << "|" << *i << "|";
		}
	return os;
}


void Clear();
void addStudent();
void addGrades();
void printAllStudents();
void saveAllStudentsToFile();
template <class T>
void appendFileWithObject(T object, std::string filename);
void readAllStudentsFromFile();
void sortStudentsbyFacNum();
void sortStudentsByAvGrade();
void getStudentByFacultyNum();
void showFromDatabase();
void addStudentsToDatabase(std::string studentName, int studentFacNum, std::vector<int> studentGrades, int stAvGrade);
void addGradeToStudentDatabase(int neededFacNumber, std::vector<std::string> grades);
int checkForExistingFac(int studentFacNum);
void dropAllStudents();

//Vector for dynamicly saving students info:
std::vector<Student> students;
bool sortedByFacNum = false;

MYSQL* conn;
MYSQL_RES* Result;
long RowsReturned;
int qstate; //variable for mysql


int main()
{	
	std::cout << std::endl << "Make a choice:" << std::endl;
	std::cout << "Enter 1 - Add a student and save in database." << std::endl;
	std::cout << "Enter 2 - Add grades to a student." << std::endl;
	std::cout << "Enter 3 - Get all students info." << std::endl;
	std::cout << "Enter 4 - Save all current students to file." << std::endl;
	std::cout << "Enter 5 - Read all students from file." << std::endl;
	std::cout << "Enter 6 - Sort students by faculty number." << std::endl;
	std::cout << "Enter 7 - Sort students by average grade." << std::endl;
	std::cout << "Enter 8 - Get student by faculty number." << std::endl;
	std::cout << "Enter 9 - Show all students from database." << std::endl;
	std::cout << "Enter 10 - Clear the console." << std::endl;
	std::cout << "Enter 11 to exit" << std::endl;
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1: {
		addStudent();
		break;
	}
	case 2: {
		addGrades();
		break;
	}
	case 3: {
		printAllStudents();
		break;
	}
	case 4: {
		saveAllStudentsToFile();
		break;
	}
	case 5: {
		readAllStudentsFromFile();
		break;
	}
	case 6: {
		sortStudentsbyFacNum();
		break;
	}
	case 7: {
		sortStudentsByAvGrade();
		break;
	}
	case 8: {
		getStudentByFacultyNum();
		break;
	}
	case 9: {
		showFromDatabase();
		break;
	}
	case 10: {
		Clear();
		main();
	}
	case 11: {
		exit(0);
		break;
	}
	case 66: {
		//This is order 66 it deletes all students from the database.
		dropAllStudents();
	}
	default: {
		std::cout << "Enter correct number!" << std::endl;
	}
	}
	main();
}


void Clear()
{
#if defined _WIN32
	system("cls");
#endif
}


void addStudent() {
	std::string studentName;
	std::cout << "Enter student name:" << std::endl;
	std::cin >> studentName;

	int studentFacNum;
	jump:
	std::cout << "Enter student faculty number:" << std::endl;
	std::cin >> studentFacNum;

	int existingFac = 0;
	existingFac = checkForExistingFac(studentFacNum);

	if (existingFac) {
		std::cout << "There is a student with this faculty number! Please try again!" << std::endl;
		goto jump;

	}
	std::string gradesCount;
	jump3:
	std::cout << "Enter number of grades:" << std::endl;
	std::cin >> gradesCount;

	//check for invalid input:
	for (char const& c : gradesCount) {
		if (std::isdigit(c) == 0) {
			std::cout << "The entered input should be a number!" << std::endl;
			goto jump3;
		}
	}

	//For parsing string to int.
	int gradesCountNumber;
	std::stringstream ss;
	ss << gradesCount;
	ss >> gradesCountNumber;


	std::vector<int> studentGrades;
	std::cout << "Enter all " << gradesCountNumber << " current student grades:" << std::endl;
	for (int i = 1; i <= gradesCountNumber; i++) {
		int grade;
		jump2:
		std::cout << "Enter grade " << i << " to continue." << std::endl;
		
		std::cin >> grade;
		if (grade < 2 || grade > 6) {
			std::cout << "The grade must be a number between 2 - 6!" << std::endl;

			goto jump2;
		}

		studentGrades.push_back(grade);
	}

	Student student(studentName, studentFacNum, studentGrades);
	students.push_back(student);

	std::cout << "\nYou have successfully added a new student!\n" << std::endl;

	for (Student stdnt : students) {
		std::cout << stdnt.getName() << ", " << stdnt.getId() << std::endl;
		std::cout << "Grades: ";
		stdnt.showGrades();
		std::cout << std::endl;
	}
	int stAvGrade = student.AverageGrade();

	addStudentsToDatabase(studentName, studentFacNum, studentGrades, stAvGrade);

}

void addGrades() {
	//if (students.size() == 0) {
	//	std::cout << "No students added yet!" << std::endl;
	//	main();
	//}

	int neededFacNumber;
	jump:
	std::cout<<"Facuty number of the student of which you want to add grades." <<std::endl;
	std::cin >> neededFacNumber;

	int gradesCount;
	bool foundStudent = false;
	std::vector<std::string> grades;
	for (Student stdnt : students) {
		if (stdnt.getId() == neededFacNumber) {
			foundStudent = true;
			std::cout << "The following student is chosen: " << std::endl;
			stdnt.printAllInfo();

			std::cout << "\nEnter how many grades you want to add: " << std::endl;
			std::cin >> gradesCount;

			std::cout << "Enter all " << gradesCount << " grades to the existing student:" << std::endl;
			for (int i = 1; i <= gradesCount; i++) {

				int grade;
				std::cout << "Enter grade " << i << " to continue." << std::endl;
				std::cin >> grade;
				stdnt.addNewGrade(grade);
				std::string gradeToStr = std::to_string(grade);
				grades.push_back(gradeToStr);

				bool isFound = false;
				std::vector <Student>::iterator it3;
				for (it3 = students.begin(); it3 != students.end(); ++it3) {
					if (it3->getId() == stdnt.getId()) {

						students.erase(it3);
						break;

					}
				}
				students.push_back(stdnt);
				//break;
			}
			//break;
		}


		addGradeToStudentDatabase(neededFacNumber, grades);
	}

	if (!foundStudent) {
		std::cout << "There is no student with this faculty number! Please try again!" << std::endl;
		goto jump;
	}

	if (gradesCount == 1) {
		std::cout << "Grade added sucessfully!" << std::endl;
	}
	else {
		std::cout << "Grades added sucessfully!" << std::endl;
	}
}

void printAllStudents() {

	if (students.size() == 0) {
		std::cout << "No students added yet!" << std::endl;
		main();
	}

	for (Student stdnt : students) {
		std::cout << stdnt.getName() << ", " << stdnt.getId() << std::endl;
		std::cout << "Grades: ";
		stdnt.showGrades();
		std::cout << std::endl;
		stdnt.AverageGrade();
	}
}


void saveAllStudentsToFile() {

	if (students.size() == 0) {
		std::cout << "There are no students to save!" << std::endl;
		main();
	}
	for (Student stdnt : students) {
		appendFileWithObject(stdnt, "students.txt");
	}

	students.clear();
	std::cout << "Students successfully saved in file!"<< std::endl;

}

template <class T>
void appendFileWithObject(T object, std::string filename) {
	std::ofstream outfile;
	outfile.open(filename, std::ios::app);
	if (outfile.fail()) {
		std::cout << "File was not found\n";
		system("PAUSE");
		exit(1);
	}
	outfile << object << std::endl;
	outfile.close();
}


void readAllStudentsFromFile() {
	std::ifstream infile;
	infile.open("students.txt", std::ios::in);
	if (infile.fail()) {
		std::cout << "File was not found\n";
		system("PAUSE");
		exit(1);
	}
	std::vector<Student> studentes;

	std::string studentId;
	std::string studentName;
	std::string ignored;
	std::string averageGrade;
	std::string stringGrades;
	std::vector<int> grades;
	std::string s;

	while (infile >> ignored >> studentId >> ignored >> studentName >> ignored >> ignored >> averageGrade >> ignored >> ignored >> stringGrades) {
		std::cout << "StudentID: " << studentId << " Name: " << studentName << " Average Grade: " << averageGrade <<
			" All grades: " << stringGrades << std::endl;
	}
}

void sortStudentsbyFacNum() {
	//SELECTION SORT
	for (int i = 0; i < students.size() - 1; i++) {
		int min_idx = i;
		for (int j = i + 1; j < students.size(); j++) {
			if (students[j].getId() < students[min_idx].getId()) {
				min_idx = j;
			}

			Student temporary = students[min_idx];
			students[min_idx] = students[i];
			students[i] = temporary;
		}
	}
	sortedByFacNum = true;
	std::cout << "Students sorted dynamically by faculty number, using selection sort. " << std::endl;
}


void sortStudentsByAvGrade() {
		//INSERTION SORT
	int n = students.size();
	for (int i = 1; i < n; ++i) {
		Student key = students[i];
		int j = i - 1;

		while (j >= 0 && students[j].AverageGrade() > key.AverageGrade()) {
			students[j + 1] = students[j];
			j = j - 1;
		}
		students[j + 1] = key;
	}

	std::cout << "Students sorted dynamically by average grade, using insertion sort. " << std::endl;
}


void getStudentByFacultyNum() {
	
	if (students.size() == 0) {
		std::cout << "There are no students!" << std::endl;
		main();
	}

	int facNum;
	jump:
	std::cout << "Enter the faculty number of the student you want to find: " << std::endl;
	std::cin >> facNum;

	for (Student stdnt : students) {
		if (stdnt.getId() == facNum) {
			if (sortedByFacNum) {
				int min = 0;
				int max = students.size() - 1;
				while (min <= max) {
					int mid = (min + max) / 2;
					if (facNum == students[mid].getId()) {
						stdnt.printAllInfo();
						main();
					}else if(facNum > students[mid].getId()){
						max = mid - 1;
					}
					else {
						min = mid + 1;
					}
				}
			}
			else {
				stdnt.printAllInfo();
				std::cout << "Student printed successfully!" << std::endl;
			}
		}
		else {
			std::cout << "Student with such faculty number doesn't exist." << std::endl;
			std::cout << "Try again!" << std::endl;
			goto jump;
		}
	}
}



void showFromDatabase() {
	
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, NULL, 0);

	if (conn) {
		puts("Successfull connection to database!");

		std::string query = "SELECT * FROM test";
		const char* q = query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate) {
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res)) {
				printf("ID: %s, Name: %s, Average Grade: %s, Grades: %s\n", row[0], row[1], row[2], row[3]);
			}
		}
		else {
			std::cout << "Query failed:" << mysql_error(conn) << std::endl;
		}
	}
	else {
		puts("Connection to database failed!");
	}
}

void addStudentsToDatabase(std::string studentName, int studentFacNum, std::vector<int> studentGrades, int stAvGrade){
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, NULL, 0);


	//system("cls");
	std::stringstream ss;
	for (size_t i = 0; i < studentGrades.size(); ++i)
	{
		if (i != 0)
			ss << ", ";
		ss << studentGrades[i];
	}
	std::string gradesString = ss.str();
	std::string insertQuery = "INSERT INTO test (id, name, averageGrade, grades) VALUES (" + std::to_string(studentFacNum) + ", '" + studentName + "', " + std::to_string(stAvGrade) + ", '" + gradesString + "')";

	const char* q = insertQuery.c_str();

	qstate = mysql_query(conn, q);

	if (!qstate) {
		std::cout << "Saved to database success!" <<std::endl;
	}else{
		std::cout << "Querry exec problem!" << mysql_errno(conn) << std::endl;
	}
}


void addGradeToStudentDatabase(int neededFacNumber, std::vector<std::string> grades) {
	conn = mysql_init(0);

	//conn = mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, NULL, 0);
	//std::string insertQuery = "UPDATE test SET grades = CONCAT(grades, '" + std::to_string(neededFacNumber) + "') Where" + std::to_string(neededFacNumber);

	//const char* q = insertQuery.c_str();
	//qstate = mysql_query(conn, q);


}

int checkForExistingFac(int studentFacNum) {
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, NULL, 0);
	std::string Query = "SELECT id from test WHERE id = '" + std::to_string(studentFacNum) + "'";

	const char* q = Query.c_str();

	qstate = mysql_query(conn, q);

	Result = mysql_store_result(conn);
	if (Result) {
		//rowsReturned shows the current found rows.
		RowsReturned = mysql_num_rows(Result);
		if (RowsReturned > 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

//This is hidden from the user function. He should be not able to use it, because it deletes all rows in the test table. 
void dropAllStudents() {
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "localhost", "root", "password", "testdb", 3306, NULL, 0);
	std::string Query = "DELETE FROM test";

	const char* q = Query.c_str();

	qstate = mysql_query(conn, q);
}