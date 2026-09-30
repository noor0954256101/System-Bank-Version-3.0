#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

string FilleClient = "ClientData.txt";
string FilleUsers = "UsersDate.txt";

void MainMenueScreen();
void TransactionsClientScreen();
void ManageUsersScreen();
void Login();
void GoBackToMainMenueScrean();


enum eBankClient { ShowClientList = 1, AddNewClient = 2, DeleteClient = 3, UpdateClientInfo = 4, FindClient = 5, transactions = 6, ManageUsers=7, Logout = 8 };

enum enTransactions { Deposit = 1, Withdraw = 2, TotalBalances = 3, MainMenue = 4 };

enum enManageUsers { ListUsers = 1, AddNewUser = 2, DeleteUsers = 3, UpdateUsers = 4, FindUsers = 5, MainMenueS = 6 };

enum enPermation { All = -1, PListClient = 1, PAddClient = 2, PDeletClient = 4, PUpdateClient = 8, PFindClient = 16, Ptransactions = 32, PManageUsers = 64 };

struct sClient
{
	string Account;
	string PinCode;
	string Name;
	string Phone;
	double Balance;
	bool MarkForDelete = false;
};

struct sUsers
{
	string UsersName;
	string Password;
	int Permisstion;
	bool MarkForDelete = false;
};

sUsers CurentUser;

bool IsPerission(enPermation Permisstions)
{
	if (CurentUser.Permisstion == enPermation::All)
		return true;

	if ((CurentUser.Permisstion&Permisstions )== Permisstions)
		return true;
	else
		return false;
}

void AccessDeniedScrean()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Dothis, \nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}

vector <string> SplitWord(string S1, string Delim)
{
	string sWord;
	short Pos = 0;
	vector <string> vsword;
	while ((Pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != "")
		{
			vsword.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}
	if (S1 != "")
		vsword.push_back(S1);

	return vsword;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitWord(Line, Seperator);
	Client.Account = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.Balance = stod(vClientData[4]);
	return Client;
}

sUsers ConvertLinetoRecordUsers(string Line, string Seperator = "#//#")
{
	sUsers User;
	vector<string> vUserData;

	vUserData = SplitWord(Line, Seperator);

	User.UsersName = vUserData[0];
	User.Password = vUserData[1];
	User.Permisstion = stod(vUserData[2]);

	return User;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}

vector <sUsers> LoadUsersDataFromFile(string FileName)
{
	vector <sUsers> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sUsers User;
		while (getline(MyFile, Line))
		{
			User = ConvertLinetoRecordUsers(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
}

void PrintInfoClient(sClient Client)
{
	cout << left << "| " << setw(18) << Client.Account	<< "| " << setw(12) << Client.PinCode<< "| " << setw(30) << Client.Name	<< "| " << setw(13) << Client.Phone	<< "| " << setw(8) << Client.Balance	<< endl;
}

void PrintInfoUsers(sUsers User)
{
	cout << left << "| " << setw(18) << User.UsersName<< "| " << setw(12) << User.Password<< "| " << setw(30) << User.Permisstion<< endl;
}

void ShowClientListScreen(vector <sClient>& vClient)
{
	if (!IsPerission(enPermation::PListClient))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	cout << "                                      Client List ( " << vClient.size() << " ) Client(s).\n\n";
	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
	cout << left << "| " << setw(18) << "Account Number" << "| " << setw(12) << "Pin Code" << "| " << setw(30) << "Client Name" << "| " << setw(13) << "Phone" << "| " << setw(8) << "Balance" << endl;
	cout << left << "\n-----------------------------------------------------------------------------------------------------\n\n";
	for (sClient Client : vClient)
	{
		PrintInfoClient(Client);
		cout << endl;
	}
	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
}

bool FindClientAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	for (sClient C : vClients)
	{
		if (C.Account == AccountNumber)
		{
			return true;
		}
	}
	return false;
}

bool FindUserUserName(string UserName, vector<sUsers>& vUsers)
{
	for (sUsers C : vUsers)
	{
		if (C.UsersName == UserName)
		{
			return true;
		}
	}
	return false;
}

sClient ReadNewClient(vector<sClient>& vClient)
{
	sClient Client;
	cout << "Enter Account Number ? ";
	getline(cin >> ws, Client.Account);
	while (FindClientAccountNumber(Client.Account, vClient))
	{
		cout << "Client With [" << Client.Account << "] already exists!, Enter another Account Number :";
		getline(cin >> ws, Client.Account);
	}
	cout << "Enter PinCode ? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ?";
	getline(cin, Client.Phone);

	cout << "Enter AccuntBalance ? ";
	cin >> Client.Balance;
	return Client;
}

sUsers ReadNewUser(vector<sUsers>& vUser)
{
	sUsers User;
	cout << "Enter User Name ? ";
	getline(cin >> ws, User.UsersName);
	while (FindUserUserName(User.UsersName, vUser))
	{
		cout << "Client With [" << User.UsersName << "] already exists!, Enter another Account Number :";
		getline(cin >> ws, User.UsersName);
	}
	cout << "Enter Password ? ";
	getline(cin, User.Password);

	int Perm=0;
	char qu = 'n';

	cout << "Do you went to give full access ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		User.Permisstion = enPermation::All;
		return User;
	}

	cout << "\n\nwhat Do you went to give access ?";

	cout << "\nShow Client List ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PListClient;
	}

	cout << "\nAdd New Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PAddClient;
	}

	cout << "\nDelete Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PDeletClient;
	}

	cout << "\UpDate Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PUpdateClient;
	}

	cout << "\Find Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PFindClient;
	}

	cout << "\nTransactions Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::Ptransactions;
	}

	cout << "\nManageUsers Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PManageUsers;
	}
	User.Permisstion = Perm;
	return User;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.Account + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.Balance);
	return stClientRecord;
}

string ConvertRecordToLine(sUsers User, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += User.UsersName + Seperator;
	stClientRecord += User.Password + Seperator;
	stClientRecord += to_string(User.Permisstion) + Seperator;
	return stClientRecord;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

vector <sUsers> SaveUsersDataToFile(string FileName, vector<sUsers>& vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;

	if (MyFile.is_open())
	{
		for (sUsers C : vUser)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUser;
}

void AddLineToFile(string Line, string NameFile)
{
	fstream MyFille;
	MyFille.open(NameFile, ios::out | ios::app);

	if (MyFille.is_open())
	{
		MyFille << Line << endl;
		MyFille.close();
	}
}

void AddNewClients(vector <sClient>& vClient)
{
	sClient Client;
	Client = ReadNewClient(vClient);
	AddLineToFile(ConvertRecordToLine(Client), FilleClient);
	vClient = LoadCleintsDataFromFile(FilleClient);
}

void AddNewUsers(vector <sUsers>& vUser)
{
	sUsers User;
	User = ReadNewUser(vUser);
	AddLineToFile(ConvertRecordToLine(User), FilleUsers);
	vUser = LoadUsersDataFromFile(FilleUsers);
}

void AddClients(vector <sClient>& vClient)
{
	char Yes = 'Y';
	do
	{
		cout << "\nAdding New Client : \n\n";
		AddNewClients(vClient);
		cout << "\nClient Added Successfully , Do Youwant to add more clients ?\n";
		cin >> Yes;

	} while (toupper(Yes) == 'Y');
}

void AddUsers(vector <sUsers>& vUser)
{
	char Yes = 'Y';
	do
	{
		cout << "\nAdding New User : \n\n";
		AddNewUsers(vUser);
		cout << "\User Added Successfully , Do Youwant to add more Users ?\n";
		cin >> Yes;

	} while (toupper(Yes) == 'Y');
}

string ReadAccountNumber()
{
	string Account;
	cout << "Enter Account Number : " << endl;
	cin >> Account;
	return Account;
}

string ReadUserName()
{
	string UserName;
	cout << "Enter User Name : " << endl;
	cin >> UserName;
	return UserName;
}

string ReadPassword()
{
	string Password;
	cout << "Enter Password : " << endl;
	cin >> Password;
	return Password;
}

void PrintClientRecord(sClient Client)
{
	cout << "\n\nThe following is the extracted client record:\n";
	cout << "-------------------------------------------------------\n";
	cout << "\nAccout Number  : " << Client.Account;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.Balance;
	cout << "\n\n-------------------------------------------------------\n\n";
}

void PrintUserRecord(sUsers User)
{
	cout << "\n\nThe following is the extracted client record:\n";
	cout << "-------------------------------------------------------\n";
	cout << "\nUser Name    : " << User.UsersName;
	cout << "\nPassword     : " <<User.Password;
	cout << "\nPermition    : " << User.Permisstion;
	cout << "\n\n-------------------------------------------------------\n\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient>& vClients, sClient& Client)
{
	for (sClient C : vClients)
	{
		if (C.Account == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUserName(string UserName, vector<sUsers>& vUser, sUsers& User)
{
	for (sUsers C : vUser)
	{
		if (C.UsersName == UserName)
		{
			User = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUserNameAndPassWord(string UserName, string PassWord,sUsers& User)
{

	vector<sUsers>vUser = LoadUsersDataFromFile(FilleUsers);

	for (sUsers C : vUser)
	{
		if (C.UsersName == UserName && C.Password == PassWord)
		{
			User = C;
			return true;
		}
	}
	return false;
}

bool LoadUserInfo(string UserName, string PassWord)
{
	if (FindUserByUserNameAndPassWord(UserName,PassWord,CurentUser))
		return true;
	else
		return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.Account == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector <sUsers>& vUsers)
{
	for (sUsers& C : vUsers)
	{
		if (C.UsersName == UserName)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(FilleClient, vClients);
			vClients = LoadCleintsDataFromFile(FilleClient);
			cout << "\n\nClient Deleted Successfully.";
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
		return false;
	}
}

bool DeleteUserByUserName(string UserName, vector<sUsers>& vUser)
{
	sUsers User;
	char Answer = 'n';
	if (FindUserByUserName(UserName, vUser, User))
	{
		PrintUserRecord(User);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkUserForDeleteByUserName(UserName, vUser);
			SaveUsersDataToFile(FilleUsers, vUser);
			vUser = LoadUsersDataFromFile(FilleUsers);
			cout << "\n\nUser Deleted Successfully.";
		}
	}

	else
	{
		cout << "\nClient with User Name (" << UserName	 << ") is Not Found!";
		return false;
	}
}

void UpDateInfoClientInVector(string AccountNumber, vector<sClient>& vClients, sClient UpdateClient)
{
	for (short i = 0; i < vClients.size(); i++)
	{
		if (vClients[i].Account == AccountNumber)
		{
			vClients[i].Balance = UpdateClient.Balance;
			vClients[i].Name = UpdateClient.Name;
			vClients[i].Phone = UpdateClient.Phone;
			vClients[i].PinCode = UpdateClient.PinCode;
			return;
		}
	}
}

void UpDateInfoUserInVector(string UserName, vector<sUsers>& vUsers, sUsers UpdateUser)
{
	for (short i = 0; i < vUsers.size(); i++)
	{
		if (vUsers[i].UsersName == UserName)
		{
			vUsers[i].Password = UpdateUser.Password;
			vUsers[i].Permisstion = UpdateUser.Permisstion;
			return;
		}
	}
}

sClient EnterUpdateForClient()
{
	sClient Client;

	cout << "\n\nEnter PinCode ? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ?";
	getline(cin, Client.Phone);

	cout << "Enter AccuntBalance ? ";
	cin >> Client.Balance;
	return Client;
}

sUsers EnterUpdateForUser()
{
	sUsers User;
	cout << "Enter Password ? ";
	cin >> User.Password;

	int Perm = 0;
	char qu = 'n';

	cout << "\n\nDo you went to give full access ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		User.Permisstion = enPermation::All;
		return User;
	}

	cout << "\n\nwhat Do you went to give access ?";

	cout << "\nShow Client List ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PListClient;
	}

	cout << "\nAdd New Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PAddClient;
	}

	cout << "\nDelete Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PDeletClient;
	}

	cout << "\UpDate Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PUpdateClient;
	}

	cout << "\Find Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PFindClient;
	}

	cout << "\nTransactions Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::Ptransactions;
	}

	cout << "\nManageUsers Client ? y/n ?";
	cin >> qu;
	if (qu == 'y' || qu == 'Y')
	{
		Perm += enPermation::PManageUsers;
	}
	User.Permisstion = Perm;
	return User;
}

bool UpDateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
		cout << "\n\nAre you sure you want Updata this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			UpDateInfoClientInVector(AccountNumber, vClients, EnterUpdateForClient());
			SaveCleintsDataToFile(FilleClient, vClients);
			vClients = LoadCleintsDataFromFile(FilleClient);
			cout << "\n\nClient UpDated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
		return false;
	}
}

bool UpDateUserByUserName(string UserName, vector<sUsers>& vUser)
{
	sUsers User;
	char Answer = 'n';
	if (FindUserByUserName(UserName, vUser, User))
	{
		PrintUserRecord(User);
		cout << "\n\nAre you sure you want Update this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			UpDateInfoUserInVector(UserName, vUser, EnterUpdateForUser());
			SaveUsersDataToFile(FilleUsers, vUser);
			vUser = LoadUsersDataFromFile(FilleUsers);
			cout << "\n\nUser UpDate Successfully.";
		}
	}
	else
	{
		cout << "\nClient with User Name (" << UserName	<< ") is Not Found!";
		return false;
	}
}

void DisplayClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientRecord(Client);
	else
		cout << "\nClient with Account Number ("<< AccountNumber<< ") is Not Found!";
}

void DisplayUserByUserName(string UserName, vector<sUsers>& vUser)
{
	sUsers User;
	if (FindUserByUserName(UserName, vUser, User))
		PrintUserRecord(User);
	else
		cout << "\nClient with User Name ("	<< UserName	<< ") is Not Found!";
}

void DeleteClientScreen(vector<sClient>& vClient)
{

	if (!IsPerission(enPermation::PDeletClient))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Delete Clients Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	DeleteClientByAccountNumber(ReadAccountNumber(), vClient);
}

void DeleteUserScreen(vector<sUsers>& vUser)
{
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Delete Users Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	DeleteUserByUserName(ReadUserName(), vUser);
}

void AddNewClientScreen(vector<sClient>& vClient)
{
	if (!IsPerission(enPermation::PAddClient))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Add New Clients Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	AddClients(vClient);
}

void AddNewUsersScreen(vector<sUsers>& vUsers)
{
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Add New User Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	AddUsers(vUsers);
}

void UpdateClientInfoScreen(vector<sClient>& vClient)
{
	if (!IsPerission(enPermation::PUpdateClient))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Update Clients Info Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	UpDateClientByAccountNumber(ReadAccountNumber(), vClient);
}

void UpdateUserInfoScreen(vector<sUsers>& vUser)
{
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Update Users Info Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	UpDateUserByUserName(ReadUserName(), vUser);
}

void FindClientScreen(vector<sClient>& vClient)
{
	if (!IsPerission(enPermation::PFindClient))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Find Clients Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	DisplayClientByAccountNumber(ReadAccountNumber(), vClient);
}

void FindUserScreen(vector<sUsers>& vUser)
{

	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Find Users Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";

	DisplayUserByUserName(ReadUserName(), vUser);
}

short ReadTransactionOption()
{
	short Num;
	cout << "Choose What do you want to do? [1 to 4] ";
	cin >> Num;
	return Num;
}

short ReadManageUsersOption()
{
	short Num;
	cout << "Choose What do you want to do? [1 to 6] ";
	cin >> Num;
	return Num;
}

void GoBackToMainMenueScrean()
{
	cout << "\n\n\nCliek any Key to go to The Main Manue Screan....";
	system("pause>0");
	system("cls");
	MainMenueScreen();
}

void GoBackToTransactionsMenueScrean()
{
	cout << "\n\n\nCliek any Key to go to The Main Manue Screan....";
	system("pause>0");
	system("cls");
	TransactionsClientScreen();
}

void GoBackToManegeusersMenueScrean()
{
	cout << "\n\n\nCliek any Key to go to The Main Manue Screan....";
	system("pause>0");
	system("cls");
	ManageUsersScreen();
}

short ReadMainMenueOption()
{
	short Num;
	cout << "Choose What do you want to do? [1 to 8] ";
	cin >> Num;
	return Num;
}

void ShowUserInfoInSerean(vector<sClient>& vClient, eBankClient Result)
{
	switch (Result)
	{
	case eBankClient::ShowClientList:
		system("cls");
		ShowClientListScreen(vClient);
		GoBackToMainMenueScrean();
		break;

	case eBankClient::AddNewClient:
		system("cls");
		AddNewClientScreen(vClient);
		GoBackToMainMenueScrean();
		break;

	case eBankClient::DeleteClient:
		system("cls");
		DeleteClientScreen(vClient);
		GoBackToMainMenueScrean();
		break;

	case eBankClient::UpdateClientInfo:
		system("cls");
		UpdateClientInfoScreen(vClient);
		GoBackToMainMenueScrean();
		break;

	case eBankClient::FindClient:
		system("cls");
		FindClientScreen(vClient);
		GoBackToMainMenueScrean();
		break;

	case eBankClient::transactions:
		system("cls");
		TransactionsClientScreen();
		GoBackToMainMenueScrean();
		break;

	case eBankClient::ManageUsers:
		system("cls");
		ManageUsersScreen();
		GoBackToMainMenueScrean();
		break;

	case eBankClient::Logout:
		system("cls");
		Login();
	}
}

void Depositinfo(string AccountNumber, vector<sClient>& vClients)
{
	short depositAmount;
	char Answer = 'n';
	sClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
		cout << "Enter deposit amount : ";
		cin >> depositAmount;
		cout << "\n\nAre you sure you want Deposit this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			Client.Balance += depositAmount;

			for (sClient& C : vClients)
			{
				if (C.Account == AccountNumber)
				{
					C = Client;
					break;
				}

			}
			SaveCleintsDataToFile(FilleClient, vClients);
			vClients = LoadCleintsDataFromFile(FilleClient);
			cout << "\n\nClient Deposit Successfully.\n\nBalance Now is : " << Client.Balance << endl;
		}
	}
	else
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}

void DepositClientScreen(vector<sClient>& vClient)
{
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Deposit Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	Depositinfo(ReadAccountNumber(), vClient);
}

void Withdrawinfo(string AccountNumber, vector<sClient>& vClients)
{
	short WithdrawAmount;
	char Answer = 'n';
	sClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientRecord(Client);
		cout << "Enter Withdraw amount : ";
		cin >> WithdrawAmount;
		while (WithdrawAmount > Client.Balance)
		{
			cout << "\n\nThe Balance is :" << Client.Balance << "\n\nPlease Enter amount Up to :" << Client.Balance << endl;

			cout << "\nEnter Withdraw amount : ";
			cin >> WithdrawAmount;
		}
		cout << "\n\nAre you sure you want Withdraw this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			Client.Balance -= WithdrawAmount;
			for (sClient& C : vClients)
			{
				if (C.Account == AccountNumber)
				{
					C = Client;
					break;
				}
			}
			SaveCleintsDataToFile(FilleClient, vClients);
			vClients = LoadCleintsDataFromFile(FilleClient);
			cout << "\n\nClient WithDraw Successfully.\n\nBalance Now is : " << Client.Balance << endl;
		}
	}
	else
		cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
}

void WithdrawClientScreen(vector<sClient>& vClient)
{
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";
	cout << "\t   Withdraw Screen\n";
	cout << "- - - - - - - - - - - - - - - - - - - - -\n";

	Withdrawinfo(ReadAccountNumber(), vClient);
}

void PrintInfoClientTotalBalance(sClient Client)
{
	cout << left << "| " << setw(18) << Client.Account
		<< "| " << setw(30) << Client.Name
		<< "| " << setw(8) << Client.Balance
		<< endl;
}

void ShowTotalBalanceScreen(vector <sClient>& vClient)
{
	int Total = 0;
	cout << "                                      Client List ( " << vClient.size() << " ) Client(s).\n\n";
	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
	cout << left << "| " << setw(18) << "Account Number" << "| " << setw(30) << "Client Name" << "| " << setw(8) << "Balance" << endl;
	cout << left << "\n-----------------------------------------------------------------------------------------------------\n\n";

	for (sClient Client : vClient)
	{
		PrintInfoClientTotalBalance(Client);
		Total += Client.Balance;
		cout << endl;
	}

	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
	cout << "\n\t\t\t\t\tTotal Balance: " << Total << endl;
}

void ShowUserInfoTransactionsInSerean(vector<sClient>& vClient, enTransactions Result)
{
	switch (Result)
	{
	case enTransactions::Deposit:

		system("cls");
		DepositClientScreen(vClient);
		GoBackToTransactionsMenueScrean();
		break;
	case enTransactions::Withdraw:
		system("cls");
		WithdrawClientScreen(vClient);
		GoBackToTransactionsMenueScrean();
		break;
	case enTransactions::TotalBalances:
		system("cls");
		ShowTotalBalanceScreen(vClient);
		GoBackToTransactionsMenueScrean();
		break;
	case enTransactions::MainMenue:
		system("cls");
		MainMenueScreen();
		break;
	}
}

void ShowUserstListScreen(vector <sUsers>& vUsers)
{
	cout << "                                      Users List ( " << vUsers.size() << " ) Users(s).\n\n";
	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
	cout << left << "| " << setw(10) << "User Name" << "| " << setw(12) << "Password" << "| " << setw(30) << "Permissions" << endl;
	cout << left << "\n-----------------------------------------------------------------------------------------------------\n\n";

	for (sUsers Users : vUsers)
	{
		PrintInfoUsers(Users);
		cout << endl;
	}
	cout << left << "-----------------------------------------------------------------------------------------------------\n\n";
}

void ShowUserInfoManageUsersInSerean(vector<sUsers>& vUsers, enManageUsers Result)
{
	switch (Result)
	{
	case enManageUsers::ListUsers:
		system("cls");
		ShowUserstListScreen(vUsers);
		GoBackToManegeusersMenueScrean();
		break;
		case enManageUsers::AddNewUser:
		system("cls");
		AddNewUsersScreen(vUsers);
		GoBackToManegeusersMenueScrean();
		break;
	case enManageUsers::DeleteUsers:
		system("cls");
		DeleteUserScreen(vUsers);
		GoBackToManegeusersMenueScrean();
		break;
	case enManageUsers::UpdateUsers:
		system("cls");
		UpdateUserInfoScreen(vUsers);
		GoBackToManegeusersMenueScrean();
		break;
		case enManageUsers::FindUsers:
		system("cls");
		FindUserScreen(vUsers);
		GoBackToManegeusersMenueScrean();
		break;
	case enManageUsers::MainMenueS:

		system("cls");
		MainMenueScreen();
		break;
	}
}

void MainMenueScreen()
{
	vector <sClient> vClient = LoadCleintsDataFromFile(FilleClient);

	system("cls");
	cout << "==============================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "==============================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "==============================================\n";
	ShowUserInfoInSerean(vClient, eBankClient(ReadMainMenueOption()));
}

void TransactionsClientScreen()
{
	if (!IsPerission(enPermation::Ptransactions))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	vector <sClient> vClient = LoadCleintsDataFromFile(FilleClient);

	system("cls");
	cout << "==============================================\n";
	cout << "\tTransactions Menue Screen\n";
	cout << "==============================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] TotalBalances.\n";
	cout << "\t[4] MainMenue.\n";
	cout << "==============================================\n";
	ShowUserInfoTransactionsInSerean(vClient, enTransactions(ReadTransactionOption()));
}

void ManageUsersScreen()
{
	if (!IsPerission(enPermation::PManageUsers))
	{
		AccessDeniedScrean();
		GoBackToMainMenueScrean();
		return;
	}

	vector <sUsers> vUsers = LoadUsersDataFromFile(FilleUsers);

	system("cls");
	cout << "==============================================\n";
	cout << "\tManage Users Menue Screen\n";
	cout << "==============================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New Users.\n";
	cout << "\t[3] Delete Users.\n";
	cout << "\t[4] Update Users.\n";
	cout << "\t[5] Find Users.\n";
	cout << "\t[6] MainMenue.\n";
	cout << "==============================================\n";
	ShowUserInfoManageUsersInSerean(vUsers, enManageUsers(ReadManageUsersOption()));
}

void Login()
{
	bool LoginFaild = false;
	string UserName, PassWord;
	do
	{
		system("cls");
		cout << "- - - - - - - - - - - - - - - - - - - - -\n";
		cout << "\t   Login Screen\n";
		cout << "- - - - - - - - - - - - - - - - - - - - -\n";

		if (LoginFaild)
		{
			cout << "Invalid UserName/PassWord ! \n";
		}

	UserName = ReadUserName();

	PassWord = ReadPassword();

	LoginFaild = !LoadUserInfo(UserName, PassWord);
	} while (LoginFaild);

	MainMenueScreen();
}

int main()
{
	Login();
	return 0;
}