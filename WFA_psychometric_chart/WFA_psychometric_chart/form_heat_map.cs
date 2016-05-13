﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Data.SQLite;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WFA_psychometric_chart
{
    public partial class form_heat_map : Form
    {
        private  Form1_main form1;
        public form_heat_map(Form1_main formx)
        {
            this.form1 = formx;
            InitializeComponent();
          this.Disposed += new System.EventHandler ( this.form_heat_map_Disposed );
        }
        int map_loaded = 0;
        int index_selected;//this is used for location services..
        private void form_heat_map_Load(object sender, EventArgs e)
        {

            chart_plot();
            
            //lets set the data time picker default values...
            dtp_From.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.MinDate = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_From.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_To.MaxDate = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            dtp_From.Value = new DateTime(DateTime.Now.Year, 1, 1);
            dtp_To.Value = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);

            PullLocationInformation();//this is for loading location information
        }

        private void PullLocationInformation()
        {
            try
            {
                /*This methods pulls the building location information..*/
                cb1_select_data.Items.Clear();
                ArrayList stored_location = new ArrayList();
                //while loading it should populate the field...
                //lets pull the vales offline values stored in db...
                //              string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                //string connString =@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\nischal\documents\visual studio 2013\Projects\WFA_psychometric_chart\WFA_psychometric_chart\T3000.mdb;Persist Security Info=True";
                // string connString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dir + @"\T3000.mdb;Persist Security Info=True";

                //                string connString = @"Data Source=GREENBIRD;Initial Catalog=db_psychrometric_project;Integrated Security=True";

                //--changing all the database to the sqlite database...
                string databasePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                string databaseFile = databasePath + @"\db_psychrometric_project.s3db";

                string connString = @"Data Source=" + databaseFile + ";Version=3;";




                // MessageBox.Show("connection string = " + connString);


                SQLiteConnection connection = new SQLiteConnection(connString);
                connection.Open();
                SQLiteDataReader reader = null;
                SQLiteCommand comm = new SQLiteCommand("SELECT * from tbl_building_location", connection);
                //command.Parameters.AddWithValue("@1", userName)
                reader = comm.ExecuteReader();
                while (reader.Read())
                {

                    string selecte_location = reader["id"].ToString() + "," + reader["country"].ToString() + "," + reader["state"].ToString() + "," + reader["city"].ToString();
                    stored_location.Add(selecte_location);
                }
                string s = "";
                for (int i = 0; i < stored_location.Count; i++)
                {
                    cb1_select_data.Items.Add(stored_location[i]);
                    s += stored_location[i] + " , \n";
                }
                // MessageBox.Show("stored place = " + s);
                comm.Dispose();
                reader.Dispose();
                connection.Close();


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        public class data_type_hum_temp
        {
            public double temp { get; set; }
            public double hum { get; set; }
           }
        List<data_type_hum_temp> hist_temp_hum_list = new List<data_type_hum_temp>();


        //chart definatioin..
        public void chart_plot()
        {
            /*
         steps:
         * 1.set x and y axis in graph
         * 2.plot red lines
         * 3.plot green lines
         * 4.plot wet bult curve red line.
         * 5.
         * 
         */

          
        }

        double min_value = 0;
        double max_value = 0;

        //lets create two arraylist to add those and store it in the arraylist
        ArrayList temperature_value = new ArrayList();
        ArrayList pg_value_from_txtfile = new ArrayList();

        Series series1 = null;
        int load_map_checker = 0;//checks weather to load a map or not
        int index_series = 0;//this index is used by  plot_on_graph_values method

        private void button1_Click(object sender, EventArgs e)
        {
            //initially resetting values to empty
            
            //resetting ends...
            DateTime fromDate = dtp_From.Value;
            DateTime toDate = dtp_To.Value;

            //--Calling part here.....
            form1.heat_map_button_click(index_selected, fromDate, toDate);





        }//closing of the buttton...

        
        int index = 0;
        

        //THi will be done later 

        private void printMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form1.printHeatMap();
        }

        private void saveAsImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            form1.saveAsImageHeatMap();

        }

        private void cb1_select_data_SelectedIndexChanged(object sender, EventArgs e)
        {
            //on change index it will select the index value or better known as selected index.
            try
            {
                index_selected = cb1_select_data.SelectedIndex + 1; //is used to identify the location and data associated with it.
                gb_select_date.Enabled = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }

      public void form_heat_map_Disposed ( object sender, System.EventArgs e )
      {
      }
    }
}