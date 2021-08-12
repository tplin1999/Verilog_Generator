# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <sstream>
# include <algorithm> // using sort and transform

using namespace std ;
// .v檔轉換成.bench檔
struct v_file_structure
{
    vector < string > basic_information ;
    vector < string > input_port ;
    vector < string > output_port ;
    vector < string > wire ;
    vector< vector < string > > logic_gate ;
} ;

v_file_structure read_process_bench_file ( ifstream & ) ;
void erase_space ( string & ) ;

string get_io_port_number ( string str ) ;

vector < string > get_gate_information ( string ) ;

void get_wire ( v_file_structure & ) ;
vector < string > get_all_signal ( v_file_structure ) ;
void delete_same ( vector < string > & ) ;
vector < int > vec_string_to_int ( vector < string > ) ;
vector < string > vec_int_to_string ( vector < int > ) ;

void output_v_file ( ofstream & , v_file_structure ) ;
string cas_vec_string ( vector < string > ) ;
string cas_vec_string ( vector < string > , vector < string > ) ;
void format_io_wire ( ofstream & , string , vector < string > ) ;
string v_gate ( vector < string > , int ) ;

void print_vector ( vector < string > ) ;
void print_2D_vector ( vector< vector < string > > ) ;

int main ( int argc , char * argv [ ] )
{
    ifstream fin ( argv [ 1 ] , ios :: in ) ;
    ofstream fout ( argv [ 2 ] , ios :: out ) ;

    struct v_file_structure vf_structure = read_process_bench_file ( fin ) ;
    output_v_file ( fout , vf_structure ) ;
    return 0 ;
}// end main

// Read and process bench file
v_file_structure read_process_bench_file ( ifstream & fin )
{
    struct v_file_structure vf_structure ;
    
    string benchmark_line_by_line ;
    int space_number = 0 ;
    
    while ( getline ( fin , benchmark_line_by_line ) )
    {
        erase_space ( benchmark_line_by_line ) ;
        
        if ( benchmark_line_by_line . length ( ) == 0 )
        {
            space_number = space_number + 1 ;
            continue ;
        }
        
        switch ( space_number )
        {
            case 0 :
                vf_structure . basic_information . push_back ( benchmark_line_by_line ) ;
                break ;
            case 1 : // INPUT(X) -> X
                vf_structure . input_port . push_back ( get_io_port_number ( benchmark_line_by_line ) ) ;
                break ;
            case 2 : // OUTPUT(X) -> X
                vf_structure . output_port . push_back ( get_io_port_number ( benchmark_line_by_line ) ) ;
                break ;
            case 3 : // X=G(1,2,3,4...) -> < G , X , 1 , 2 , 3 , 4 , ... >
                vf_structure . logic_gate . push_back ( get_gate_information ( benchmark_line_by_line ) ) ;
                break ;
            default :
                cerr << "~~~~~~~~~~code error~~~~~~~~~~" << endl ;
        }
    }// end of reading file
    fin . close ( ) ;

    get_wire ( vf_structure ) ;

    return vf_structure ;
}// end read_process_bench_file

// Erase space in a string
void erase_space ( string & str )
{
    int pos = 0 ;

    while ( ( pos = str . find ( " " , 0 ) ) >= 0 )
    {
        str = str . erase ( pos , 1 ) ;
    }
}// end erase_space

// INPUT(X) -> X or OUTPUT(X) -> X
string get_io_port_number ( string str )
{
    int open_parenthesis = str . find_first_of ( "(" , 0 ) ;
    int close_parenthesis = str . find_first_of ( ")" , 0 ) ;

    string port_number = str . substr ( open_parenthesis + 1 , close_parenthesis - open_parenthesis - 1 ) ;
    return port_number ;
}// end get_io_port_number

// X=G(1,2,3,4...) -> < G , X , 1 , 2 , 3 , 4 , ... >
vector < string > get_gate_information ( string str )
{
    vector < string > gate_information ;

    int equal_pos = str . find ( "=" , 0 ) ;
    int open_parenthesis = str . find_first_of ( "(" , 0 ) ;
    int close_parenthesis = str . find_first_of ( ")" , 0 ) ;
    
    string G_number = str . substr ( equal_pos + 1 , open_parenthesis - equal_pos - 1 ) ;
    string X_number = str . substr ( 0 , equal_pos ) ;
    gate_information . push_back ( G_number ) ;
    gate_information . push_back ( X_number ) ;

    //10=AND(1,2,3,4,5) -> 1,2,3,4,5
    string input_port_number = str . substr ( open_parenthesis + 1 , close_parenthesis - open_parenthesis - 1 ) ;

    while ( ! input_port_number . empty ( ) )
    {
        int comma_pos = input_port_number . find_first_of ( "," , 0 ) ;

        if ( comma_pos >= 0 )
        {
            string first_gate_number = input_port_number . substr ( 0 , comma_pos ) ;
            gate_information . push_back ( first_gate_number ) ;
            input_port_number = input_port_number . substr ( comma_pos + 1 ) ;
        }
        else //didn't find comma
        {
            gate_information . push_back ( input_port_number ) ;
            input_port_number . clear ( ) ;
        }
        
    }
    return gate_information ;
}// end get_gate_information

// Get wire
void get_wire ( v_file_structure & vf_structure )
{
    vector < string > wire_signal = get_all_signal ( vf_structure ) ;

    delete_same ( wire_signal ) ;

    for ( int i = 0 ; i < vf_structure . input_port . size ( ) ; i ++ )
    {
        for ( int j = 0 ; j < wire_signal . size ( ) ; j ++ )
        {
            if ( wire_signal [ j ] == vf_structure . input_port [ i ] )
            {
                wire_signal . erase ( wire_signal . begin ( ) + j ) ;
                j = 0 ;
            }
        }
    }

    for ( int i = 0 ; i < vf_structure . output_port . size ( ) ; i ++ )
    {
        for ( int j = 0 ; j < wire_signal . size ( ) ; j ++ )
        {
            if ( wire_signal [ j ] == vf_structure . output_port [ i ] )
            {
                wire_signal . erase ( wire_signal . begin ( ) + j ) ;
                j = 0 ;
            }
        }
    }

    vector < int > wire_int = vec_string_to_int ( wire_signal ) ;
    sort ( wire_int . begin ( ) , wire_int . end ( ) , less < int > ( ) ) ;
    wire_signal = vec_int_to_string ( wire_int ) ;
    vf_structure . wire = wire_signal ;
}// end get_wire

/* Change
   < NAND , 1 , 2 , 3 >
   < AND , 4 , 5 , 6 >   ->  < 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 >
   < NOR , 7 , 8 , 9 >
*/
vector < string > get_all_signal ( v_file_structure vf_structure )
{
    vector < string > all_signal ;
    for ( int i = 0 ; i < vf_structure . logic_gate . size ( ) ; i ++ )
    {
        for ( int j = 1 ; j < vf_structure . logic_gate [ i ] . size ( ) ; j ++ )
        {
            all_signal . push_back ( vf_structure . logic_gate [ i ] [ j ] ) ;
        }
    }
    return all_signal ;
}// end get_all_signal

// Delete same element in a vector
void delete_same ( vector < string > & wire_signal )
{
    for ( int i = 0 ; i < wire_signal . size ( ) ; i ++ )
    {
        for ( int j = i + 1 ; j < wire_signal . size ( ) ; j ++ )
        {
            if ( wire_signal [ i ] == wire_signal [ j ] )
            {
                wire_signal . erase ( wire_signal . begin ( ) + j ) ;
                j = i + 1 ;
            }
        }
    }
}// end delete_same

//vector <string> -> vector <int>
vector < int > vec_string_to_int ( vector < string > vec_str )
{
    vector < int > vec_int ;
    for ( int i = 0 ; i < vec_str . size ( ) ; i ++ )
    {
        int temp_int ;
        istringstream ( vec_str [ i ] ) >> temp_int ;
        vec_int . push_back ( temp_int ) ;
    }
    return vec_int ;
}// end vec_string_to_int

//vector <int> -> vector <string>
vector < string > vec_int_to_string ( vector < int > vec_int )
{
    
    vector < string > vec_str ;
    for ( int i = 0 ; i < vec_int . size ( ) ; i ++ )
    {
        stringstream ss ;
        ss << vec_int [ i ] ;
        string temp_str ;
        ss >> temp_str ;
        vec_str . push_back ( temp_str ) ;
    }
    return vec_str ;
}// end vec_int_to_string

// output .v file
void output_v_file ( ofstream & fout , v_file_structure vf_structure ) 
{
    fout << "`timescale 1ns/1ps" << endl ;
    // module cX (NX,NX,NX,NX,NX);
    fout << "module " << vf_structure . basic_information [ 0 ] . substr ( 1 ) << " (" ;
    fout << cas_vec_string ( vf_structure . input_port , vf_structure . output_port ) ;
    fout << ");" << endl << endl ;
    // "input NX,NX,NX;" and "output NX,NX,NX;" and "wire NX,NX,NX;"
    fout << "input " << cas_vec_string ( vf_structure . input_port ) << ";" << endl << endl ;
    fout << "output " << cas_vec_string ( vf_structure . output_port ) << ";" << endl << endl ;
    fout << "wire " << cas_vec_string ( vf_structure . wire ) << ";" << endl << endl ;
    // gate information
    for ( int i = 0 ; i < vf_structure . logic_gate . size ( ) ; i ++ )
    {
        fout << v_gate ( vf_structure . logic_gate [ i ] , i + 1 ) << endl ;
    }
    fout << endl << "endmodule" << endl ;

    fout . close ( ) ;
}// end output_v_file

// < 1 , 2 , 3 > -> "N1,N2,N3"
string cas_vec_string ( vector < string > vec_str )
{
    string cas_port = "" ;

    for ( int i = 0 ; i < vec_str . size ( ) ; i ++ )
    {
        if ( i % 10 == 9 && i != vec_str . size ( ) - 1 )
        {
            cas_port = cas_port + "N" + vec_str [ i ] + "," + "\n       " ;
        }
        else if ( i == vec_str . size ( ) - 1 )
        {
            cas_port = cas_port + "N" + vec_str [ i ] ;
        }
        else
        {
            cas_port = cas_port + "N" + vec_str [ i ] + "," ;
        }
    }

    return cas_port ;
}// end cas_vec_string

// < 1 , 2 , 3 > and < 4 , 5 , 6 > -> "N1,N2,N3,N4,N5,N6"
string cas_vec_string ( vector < string > vec_str_1 , vector < string > vec_str_2 )
{
    string cas_port = "" ;

    for ( int i = 0 ; i < vec_str_2 . size ( ) ; i ++ )
    {
        vec_str_1 . push_back ( vec_str_2 [ i ] ) ;
    }
    
    
    for ( int i = 0 ; i < vec_str_1 . size ( ) ; i ++ )
    {
        if ( i % 10 == 9 && i != vec_str_1 . size ( ) - 1 )
        {
            cas_port = cas_port + "N" + vec_str_1 [ i ] + "," + "\n             " ;
        }
        else if ( i == vec_str_1 . size ( ) - 1 )
        {
            cas_port = cas_port + "N" + vec_str_1 [ i ] ;
        }
        else
        {
            cas_port = cas_port + "N" + vec_str_1 [ i ] + "," ;
        }
    }

    return cas_port ;
}// end cas_vec_string

// <NAND,3,2,1> -> nand NAND2_X (N3, N1, N2);
string v_gate ( vector < string > vec_str , int gate_number )
{
    string str_gate = "" ;
    string gate_type_low = "" ;


    if ( vec_str [ 0 ] == "BUFF" )
    {
        gate_type_low = "buf" ;
    }
    else
    {
        gate_type_low = vec_str [ 0 ] ;
        transform ( gate_type_low . begin ( ) , gate_type_low . end ( ) ,\
                    gate_type_low . begin ( ) , :: tolower ) ;
    }
    
    stringstream ss ;
    stringstream ss2 ;

    ss << vec_str . size ( ) - 2 ;
    string input_number ;
    ss >> input_number ;

    ss2 << gate_number ;
    string gate_number_str ;
    ss2 >> gate_number_str ;

    str_gate = gate_type_low + " " + vec_str [ 0 ] + input_number + "_" + gate_number_str + " (" ;

    for ( int i = 1 ; i < vec_str . size ( ) ; i ++ )
    {
        if ( i == vec_str . size ( ) - 1 )
        {
            str_gate = str_gate + "N" + vec_str [ i ] + ");" ;
        }
        else
        {
            str_gate = str_gate + "N" + vec_str [ i ] + ", " ;
        }
    }

    return str_gate ;
}// end v_gate

// Print element in vector
void print_vector ( vector < string > vec )
{
    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        cout << vec [ i ] << " " ;
    }

    cout << endl ;
}// end print_vector

// Print element in 2D vector
void print_2D_vector ( vector< vector < string > > vec )
{
    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        for ( int j = 0 ; j < vec [ i ] . size ( ) ; j ++ )
        {
            cout << vec [ i ] [ j ] << " " ;
        }
        cout << endl ;
    }
}// end print_2D_vector