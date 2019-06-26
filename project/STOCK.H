//              Preliminary Class Heirarchy of QSG Version 1.00
//              To be implimented in C++ (For Dos and possibly Windows)


typedef unsigned char byte;


// class describing an individual product
class product
      {      friend indexes;
             private :
                char ean[18];
                char desc[31];
                int group;
                float markup;
                float price;
                float vat;
             public :
                void set_ean(char *);
                void set_desc(char *);
                void set_group(int);
                void set_markup(float);
                void set_price(float);
                void set_vat(byte);
                void get_ean(char *);
                void get_desc(char *);
                int get_group(void);
                float get_markup(void);
                float get_price(void);
                byte get_vat(void);
                int load_values(char *); // loads a product from file,
                                         // given its EAN code
                                         // using the indexes class
                int lookup(void); // puts up a dialog asking for an EAN code
                                  // and searches for it
        }

// class describing an individual customer
class customer
        {     friend indexes;
              private :
                 char code[6];
                 char name[30];
                 char address[4][30];
                 char contact[30];
                 long * pindex;  // list of indexes into the product file
              public :
                 customer() { pindex=NULL;}
                 void set_code(char *);
                 void set_name(char *);
                 void set_address(int,char *); // sets address line (int)
                 void set_contact(char *);
                 void add_product(long);
                 void get_code(char *);
                 void get_name(char *);
                 void get_address(int,char *); // gets address line (int)
                 void get_contact(char *);
                 int load_values(char *);  // loads a customer from file,
                                           // using the indexes class
                 int lookup(void); // puts up a dialog asking for a
                                  // customer and searches for it
                 void stock_report(void);
                 ~customer(); { delete (pindex); }
        }

class group
      {    friend indexes;
           private :
               char desc[30];
               float markup;
               float vat;
           public :
               void set_desc(char *);
               void set_markup(float);
               void set_vat(byte);
               void get_desc(char *);
               float get_markup(void);
               float get_vat(void);
               int load_values(char *);  // loads a product group  from file,
                                         // using the indexes class
               int lookup(void); // puts up a dialog asking for a
                                 // group and search

          }



// holds lists of indexes to each file
class indexes
      {     private :
               char * ean_list[18];
	       float * vat_list;
	       char * cust_list[5];
               char * group[30];
            public :
	       indexes(); // opens the master index file,
                          // loads into memory,
                          // resizing as necessary
               void extra(product);
               void extra(customer);
               void extra(float);
               void modify(product);
               void modify(customer);
               void modify(float);
               ~indexes();
      }



