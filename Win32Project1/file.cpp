#include "file.h"


CFileReader::CFileReader()
{
}


CFileReader::~CFileReader()
{
}

// ew dodaæ obs³ugê b³êdnego formatu pliku
vector<CRecord *> CFileReader::vReadData(CString sFileName /*= DEF_SOURCE_FILE*/, int iNumOfVariables /*= DEFAULT_NUM_OF_VARIABLES*/)
{
	vector<CRecord *> v_result;
	fstream fs_file;
	char c_buffor[BUFFOR_SIZE];
	string s_line;
	double d_value;
	vector<double> v_arguments;
	size_t st_iterator;
	bool b_correct_line;

	fs_file.open(sFileName);

	while (fs_file.getline(c_buffor, BUFFOR_SIZE))
	{
		s_line = c_buffor;
		b_correct_line = s_line.length() > 0;
		v_arguments.clear();

		for (int ii = 0; b_correct_line && ii < iNumOfVariables; ii++)
		{
			st_iterator = 0;
			try 
			{
				v_arguments.push_back(stod(s_line, &st_iterator));
				s_line = s_line.substr(st_iterator + 2);
			}
			catch (...)
			{
				b_correct_line = false;
			}
		}

		if (b_correct_line)
		{
			try { d_value = stod(s_line); }
			catch (...) { b_correct_line = false; }
		}

		if (b_correct_line)
			v_result.push_back(new CRecord(v_arguments, d_value, iNumOfVariables));

	}

	fs_file.close();
	return v_result;
}

void CFileReader::vSaveToFile(string sText, string sFileName /*= DEF_RESULT_FILE*/)
{
	fstream f_file;
	f_file.open(sFileName, ios::out);
	if (f_file.good())
	{
		f_file << sText;
		f_file.close();
	}
}
