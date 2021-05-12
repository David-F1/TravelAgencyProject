#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

enum tz{ ACT, CET, CST, EST, GMT, MSK, PST, UTC};// where tz is the time zone
enum da{ departure, arrival };//da "departure arrival"

//local time class
class LocalTime{
	private:
		int minute;
		tz timezone;
	public:
		LocalTime();
		LocalTime(int h, int m, tz t);
		void set_time(int h, int m);
		int get_hour();
		int get_minute();
		bool is_valid();
		friend ostream &operator<< (ostream &output,  LocalTime &t);
};
LocalTime::LocalTime(){
	timezone= CET;
	minute=-1;

}
LocalTime::LocalTime(int h, int m, tz t){
	timezone=t;
	set_time( h, m);
}

void LocalTime::set_time(int h, int m){
	if((h<24)&&(m<60)&&(h>=0)&&(m>=0)){
		minute=(h*60)+m;
	}
	else
	{
		cout<<"invalid entery !\n";
		minute=-1;
	}
}

int LocalTime::get_hour(){
	return minute/60;
}

int LocalTime::get_minute(){
	return minute%60;
}

bool LocalTime::is_valid(){
	if(minute<0){
		return false;
	}
	else
		return true;
}

// output operator
	ostream &operator<<(ostream &output,  LocalTime &t) {
		if(t.is_valid()){

			output <<setw(2)<<setfill('0')<<t.get_hour()<< ":"<<setw(2)<<setfill('0') << t.get_minute();
			return output;
		}
		else
			return output <<"     ";


		}
//end of localtime

// flight class
class flight{
	private:
		da arrivalorDeparture;//enum da
		string code,destination,checkin,gate,comment;
		LocalTime expected;
	public:
		flight(da AorD, string CD, string dest, string gt, string chk);
		~flight();
		string get_code();
		string get_destination();
		string get_gate();
		string get_checkin();
		string get_comment();
		LocalTime get_expected();
		void set_expected(LocalTime x);
		void set_comment(string comments);
		bool is_arrival();
		bool is_departure();
		void print(LocalTime t);

};
flight::flight(da AorD, string CD, string dest, string gt, string chk ){
	arrivalorDeparture=AorD;
	code=CD;
	destination=dest;
	checkin=chk;
	gate=gt;
	comment="";
}
flight::~flight(){
	cout <<"flight "<< code <<" deleted"<< endl;
}

string flight::get_checkin(){
	return checkin;
}

string flight::get_code(){
	return code;
}

string flight::get_comment(){
	return comment;
}

string flight::get_destination(){
	return destination;
}

string flight::get_gate(){
	return gate;
}

LocalTime flight::get_expected(){
	return expected;
}

bool flight::is_arrival(){
	if(arrivalorDeparture==arrival){
		return true;
	}
	else
		return false;

}

bool flight::is_departure(){
	if(arrivalorDeparture==departure){
		return true;
	}
	else
		return false;
}

void flight::set_comment(string comments){
	comment=comments;
}
void flight::set_expected(LocalTime x){
	expected= x;
}

void flight::print(LocalTime t){
	cout <<setw(10)<<left<<setfill(' ')<<code
		 <<setw(15)<<left<<setfill(' ')<<destination<<"\t\t"<<t<<"\t\t"<<expected<<"\t\t"
		 <<setw(10)<<left<<setfill(' ')<<gate
		 <<setw(10)<<left<<setfill(' ')<<checkin
		 <<setw(11)<<left<<setfill(' ')<<comment<<endl;
}
//end of flight

//airport class
class Airport{
	private:
		string name;
		flight *slot[24][60];
	public:
		Airport(string n);
		~Airport();
		void schedule(LocalTime t, flight* point);
		void comment(LocalTime t, string c);
		void delay(LocalTime t1, LocalTime t2);
		void print(da AD);
};

Airport::Airport(string n){
	name=n;
	for(int i=0; i<24; ++i){
		for(int j=0; j<60; ++j){
			slot[i][j]= nullptr;
		}
	}
}

Airport::~Airport(){
	/*for(int i=0; i<24; ++i){
		for(int j=0; j<60; ++j){
			delete slot[i][j];
		}
	}*/
}

void Airport::schedule(LocalTime t, flight* ptr){
	slot[t.get_hour()][t.get_minute()] =  ptr;


}

void Airport::comment(LocalTime t, string c){

	slot[t.get_hour()][t.get_minute()]->set_comment(c);
}

void Airport::delay(LocalTime t1, LocalTime t2){
	slot[t1.get_hour()][t1.get_minute()]->set_expected(t2);
}

void  Airport::print(da AD){//AD for enum departure or arrival
	LocalTime localTime;
	if(AD==arrival){
		cout <<endl<< name << " ARRIVALS" << endl
		<< "=============================" << endl;

		cout<<setw(10)<<left<<setfill(' ')<<"Flight"
			<<setw(26)<<setfill(' ')<<"From"
			<<setw(20)<<left<<setfill(' ')<<"Scheduled"
			<<setw(16)<<setfill(' ')<<"Expected"
			<<setw(10)<<setfill(' ')<<"Gate"
			<<setw(10)<<setfill(' ')<<"Check-in"
			<<setw(10)<<setfill(' ')<<"Comments"<<endl;

		for(int i=0; i<24; ++i){
			for(int j=0; j<60; ++j){
				if(slot[i][j] != nullptr){
					if (slot[i][j]->is_arrival()) {
                        localTime.set_time(i, j);
                        slot[i][j]->print(localTime);
                    }
			}
		}
	}
	}

	if(AD==departure){
		cout <<endl<< name << " DEPARTURES" << endl
            << "=============================" << endl;

		cout<<setw(10)<<setfill(' ')<<"Flight"
			<<setw(26)<<setfill(' ')<<"To"
			<<setw(20)<<setfill(' ')<<"Scheduled"
			<<setw(16)<<setfill(' ')<<"Expected"
			<<setw(10)<<setfill(' ')<<"Gate"
			<<setw(10)<<setfill(' ')<<"Check-in"
			<<setw(10)<<setfill(' ')<<"Comments"<<endl;

		for(int i=0; i<24; ++i){
			for(int j=0; j<60; ++j){
				if(slot[i][j] != nullptr){
					if (slot[i][j]->is_departure()) {
                        localTime.set_time(i, j);
                        slot[i][j]->print(localTime);
                    }
			}
		}
	}
	}

}
//end of airport


// the main function

int main(){
	Airport airport("DUSSELDORF AIRPORT");
	LocalTime LT(12, 40, tz::GMT);
    LocalTime LT_delay(13, 05, tz::GMT);

	flight*  flight1= new flight(arrival, "LH 2010", "Munich", "A04","" );
	flight*  flight2= new flight(arrival, "EW 9347", "Manchester", "B04", "" );
	flight*  flight3= new flight(departure, "AF 1307", "Paris", "B51", "192-194" );
	flight*  flight4= new flight(departure, "SU 2537", "Moscow", "C31", "252-255" );
	flight*  flight5= new flight(departure, "EW 9466", "London-Heathrow", "B35", "151-170");
	flight*	 flight6= new flight(departure, "LH 2011", "Munich", "A40", "115-120");
	flight*	 flight7= new flight(departure, "XQ 959", "Izmir", "C45", "240-242");


	airport.schedule(LT, flight1); // LT is the localtime
	airport.delay(LT, LT_delay);

	LT.set_time(14, 50);
    airport.schedule(LT, flight2);


    LT.set_time(9, 10);
    airport.schedule(LT, flight3);
    airport.comment(LT, "departed");

    LT.set_time(10, 40);
    airport.schedule(LT, flight4);
    airport.comment(LT, "boarding");

    LT.set_time(11, 15);
    airport.schedule(LT, flight5);
    airport.comment(LT, "");

	LT.set_time(13, 25);
    airport.schedule(LT, flight6);
    airport.comment(LT, "Code Sharing");

    LT.set_time(14, 55);
    LT_delay.set_time(15, 20);
    airport.schedule(LT, flight7);
    airport.comment(LT, "");
    airport.delay(LT, LT_delay);


    airport.print(arrival);
    airport.print(departure);
    cout<<"\n\n";

    // deleting the allocated memory "new flight"
    delete flight1;
    delete flight2;
    delete flight3;
    delete flight4;
    delete flight5;
    delete flight6;
    delete flight7;

	//return 0;

}
