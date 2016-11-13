#include "../include/FileUtilities.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>


/**
 * Return true if a string matches the given template
 * The template is of the form <prexifx><nnn...><suffix>.<extension>
 * Where prefix is a string, there are a number of digits and a suffix
 * This is a utility function to get around the fact that despite appearances to the contrary, 
 * gcc 4.8.n does NOT support regexes
 * We use this function to match input colour, depth and scene flow file names
 * @param prefix The prefix
 * @param num_digits The number of digits to match
 * @param suffix Any suffix following the digits
 * @param extension The file extension
 * @param test_string The candidate string to match
 * @return true if it matches else false
 */
bool match_file_name( const std::string& prefix, 
                        int num_digits, 
                        const std::string& suffix, 
                        const std::string& extension,
                        const std::string& test_string ) {
    bool matches = false;

    std::string error_message;

    // Must be the right length to start with. Any can be non-zero but at least one must be
    if( ( test_string.length()  > 0 ) || 
        ( prefix.length() > 0 )       || 
        ( suffix.length() > 0 )       ||
        ( extension.length() > 0 )    ||
        ( num_digits > 0 ) ) {

        int template_length = prefix.length() + num_digits + suffix.length() + 1 + extension.length();
        if( test_string.length() == template_length ) {

            if( std::equal(prefix.begin(), prefix.end(), test_string.begin() ) ) {

                // Check for the suffix too
                if( std::equal( suffix.begin(), suffix.end(), test_string.begin() + prefix.length() + num_digits )) {

                    if( std::equal(extension.begin(), extension.end(), test_string.end() - extension.length() ) ) {
                        // Check that all remaining cahracters are digits
                        std::string digits = test_string.substr(prefix.size(), num_digits );

                        matches = std::all_of( digits.begin(), digits.end(), ::isdigit );

                        if( ! matches ) {
                            error_message = "Digits region contains non-digits";
                        }

                    } else {
                        error_message = "Extension doesn't match";
                    }

                } else {
                    error_message = "Suffix doesn't match";
                }
            } else {
                error_message = "Prefix doesn't match";
            }

        } else {
            error_message = "Template can't match test string, it's the wrong length";
        }

    } else {
        error_message = "An invalid parameter was passed to match_file_name";
    }

    return matches;
}


/**
 * Process file content line by line.
 * @param file_name The name of the file to be read
 * @param processor Pointer to function taking a single string argument to process
 * @return true if processing was successful
 */
bool process_file_by_lines( const std::string& file_name, std::function<void(const std::string & )> processor) {
    // Open file
    std::ifstream f( file_name );

    // After this attempt to open a file, we can safely use perror() only
    // in case f.is_open() returns False.
    if (!f.is_open())
        perror(("error while opening file " + file_name).c_str());

    // Read the file via std::getline(). Rules obeyed:
    //   - first the I/O operation, then error check, then data processing
    //   - failbit and badbit prevent data processing, eofbit does not
    std::string line;
    while (getline(f, line)) {
        processor(line);
    }

    // Only in case of set badbit we are sure that errno has been set in
    // the current context. Use perror() to print error details.
    if (f.bad())
        perror(("error while reading file " + file_name).c_str());

    f.close();
    return true;
}

/**
 * @param file_name The file (or directory name) to test for
 * @param is_directory Set to true if it's a directory
 * @return true if the file exists
 */
bool file_exists( const std::string& file_name, bool & is_directory ) {
    bool exists = false;

    struct stat fileStat;
    if ( ! stat( file_name.c_str(), &fileStat) ) {
        exists = true;
        if ( S_ISREG(fileStat.st_mode) ) {
            is_directory = false;
        } else if ( S_ISDIR( fileStat.st_mode)  ) {
            is_directory = true;
        }
    }

    return exists;
}

/**
 * Return the list of files in a directory which meet a spefici criteria
 * which is labelled
 * @param directory The directory to search
 * @param files A vector to be filled by file names meeting the filter
 * @param filter The function which filters the files. Should return true if a file name matches the filter.
 */
void files_in_directory( const std::string& directory, std::vector<std::string>& files, std::function< bool( const char * ) > filter ) {
    // Get a directory listing#include <string>
    DIR *dir =  opendir ( directory.c_str() );
    if ( dir != NULL ) {
        struct dirent *ent;

        // Iterate over all the file names
        while ( ( ent = readdir ( dir ) ) != NULL ) {

            // Call the filter with each
            if( filter( ent->d_name ) ) {
                // Add matching file name
                files.push_back( ent->d_name );
            }
        }
        closedir (dir);
    } else {
        // Could not open directory
        std::cerr << "Problem reading directory " << directory << std::endl;
    }
}




/**
 * Read and return the last (non-empty) line of a text file
 * @param file_name The name of the file
 * @param text the string read
 * @return true if the read was successful
 */
bool read_last_line( std::string file_name, std::string& text ) {

    bool read_ok = false;

    std::ifstream fin;
    fin.open(file_name);
    if(fin.is_open()) {
        fin.seekg(-1,std::ios_base::end);                // go to one spot before the EOF

        bool keepLooping = true;
        while(keepLooping) {
            char ch;
            fin.get(ch);                            // Get current byte's data

            if((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                fin.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if(ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                fin.seekg(-2,std::ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        getline(fin,text);                      // Read the current line
        std::cout << "Result: " << text << std::endl;     // Display it

        fin.close();
        read_ok = true;
    } else {
        std::cerr << "Problem reading file " << file_name << std::endl;
    }

    return read_ok;
}