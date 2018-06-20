﻿namespace T3000.Forms
{
    partial class ProgramEditorForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProgramEditorForm));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tsTopMenu = new System.Windows.Forms.ToolStrip();
            this.cmdSend = new System.Windows.Forms.ToolStripButton();
            this.cmdClear = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdLoad = new System.Windows.Forms.ToolStripButton();
            this.cmdSave = new System.Windows.Forms.ToolStripButton();
            this.cmdRefresh = new System.Windows.Forms.ToolStripButton();
            this.cmdSettings = new System.Windows.Forms.ToolStripButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.grpCompileErrors = new System.Windows.Forms.GroupBox();
            this.gridCompileErrors = new System.Windows.Forms.DataGridView();
            this.dataGridViewTextBoxColumn3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ruler1 = new FastColoredTextBoxNS.Ruler();
            this.SettingsBag = new System.Windows.Forms.PropertyGrid();
            this.tsTopMenu.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.grpCompileErrors.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gridCompileErrors)).BeginInit();
            this.SuspendLayout();
            // 
            // tsTopMenu
            // 
            this.tsTopMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdSend,
            this.cmdClear,
            this.toolStripSeparator1,
            this.cmdLoad,
            this.cmdSave,
            this.cmdRefresh,
            this.cmdSettings});
            this.tsTopMenu.Location = new System.Drawing.Point(0, 0);
            this.tsTopMenu.Name = "tsTopMenu";
            this.tsTopMenu.Size = new System.Drawing.Size(827, 23);
            this.tsTopMenu.TabIndex = 8;
            this.tsTopMenu.Text = "toolStrip1";
            // 
            // cmdSend
            // 
            this.cmdSend.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdSend.Image = ((System.Drawing.Image)(resources.GetObject("cmdSend.Image")));
            this.cmdSend.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdSend.Name = "cmdSend";
            this.cmdSend.Size = new System.Drawing.Size(66, 20);
            this.cmdSend.Text = "Send (F2)";
            this.cmdSend.Click += new System.EventHandler(this.cmdSend_Click);
            // 
            // cmdClear
            // 
            this.cmdClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdClear.Image = ((System.Drawing.Image)(resources.GetObject("cmdClear.Image")));
            this.cmdClear.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdClear.Name = "cmdClear";
            this.cmdClear.Size = new System.Drawing.Size(67, 20);
            this.cmdClear.Text = "Clear (F4)";
            this.cmdClear.Click += new System.EventHandler(this.cmdClear_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 23);
            // 
            // cmdLoad
            // 
            this.cmdLoad.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdLoad.Image = ((System.Drawing.Image)(resources.GetObject("cmdLoad.Image")));
            this.cmdLoad.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdLoad.Name = "cmdLoad";
            this.cmdLoad.Size = new System.Drawing.Size(89, 20);
            this.cmdLoad.Text = "Load File (F7)";
            this.cmdLoad.Click += new System.EventHandler(this.cmdLoad_Click);
            // 
            // cmdSave
            // 
            this.cmdSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdSave.Image = ((System.Drawing.Image)(resources.GetObject("cmdSave.Image")));
            this.cmdSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdSave.Name = "cmdSave";
            this.cmdSave.Size = new System.Drawing.Size(87, 20);
            this.cmdSave.Text = "Save File (F6)";
            this.cmdSave.Click += new System.EventHandler(this.cmdSave_Click);
            // 
            // cmdRefresh
            // 
            this.cmdRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdRefresh.Image = ((System.Drawing.Image)(resources.GetObject("cmdRefresh.Image")));
            this.cmdRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdRefresh.Name = "cmdRefresh";
            this.cmdRefresh.Size = new System.Drawing.Size(81, 20);
            this.cmdRefresh.Text = "Refresh (F8)";
            this.cmdRefresh.Click += new System.EventHandler(this.cmdRefresh_Click);
            // 
            // cmdSettings
            // 
            this.cmdSettings.CheckOnClick = true;
            this.cmdSettings.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.cmdSettings.Image = ((System.Drawing.Image)(resources.GetObject("cmdSettings.Image")));
            this.cmdSettings.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cmdSettings.Name = "cmdSettings";
            this.cmdSettings.Size = new System.Drawing.Size(58, 20);
            this.cmdSettings.Text = "Settings";
            this.cmdSettings.Click += new System.EventHandler(this.cmdSettings_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tsTopMenu, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.flowLayoutPanel1, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.grpCompileErrors, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.ruler1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 23F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 23F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 73.11179F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 26.88822F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 37F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(827, 455);
            this.tableLayoutPanel1.TabIndex = 9;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(3, 420);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(821, 32);
            this.flowLayoutPanel1.TabIndex = 9;
            // 
            // grpCompileErrors
            // 
            this.grpCompileErrors.Controls.Add(this.gridCompileErrors);
            this.grpCompileErrors.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpCompileErrors.Location = new System.Drawing.Point(3, 320);
            this.grpCompileErrors.Name = "grpCompileErrors";
            this.grpCompileErrors.Size = new System.Drawing.Size(821, 94);
            this.grpCompileErrors.TabIndex = 10;
            this.grpCompileErrors.TabStop = false;
            this.grpCompileErrors.Text = "Compile Errors";
            // 
            // gridCompileErrors
            // 
            this.gridCompileErrors.AllowUserToAddRows = false;
            this.gridCompileErrors.AllowUserToDeleteRows = false;
            this.gridCompileErrors.ColumnHeadersHeight = 24;
            this.gridCompileErrors.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            this.gridCompileErrors.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.dataGridViewTextBoxColumn3,
            this.dataGridViewTextBoxColumn4,
            this.dataGridViewTextBoxColumn1});
            this.gridCompileErrors.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridCompileErrors.Location = new System.Drawing.Point(3, 17);
            this.gridCompileErrors.MultiSelect = false;
            this.gridCompileErrors.Name = "gridCompileErrors";
            this.gridCompileErrors.ReadOnly = true;
            this.gridCompileErrors.RowHeadersVisible = false;
            this.gridCompileErrors.RowTemplate.Height = 24;
            this.gridCompileErrors.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.gridCompileErrors.Size = new System.Drawing.Size(815, 74);
            this.gridCompileErrors.TabIndex = 3;
            this.gridCompileErrors.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.gridCompileErrors_CellDoubleClick);
            // 
            // dataGridViewTextBoxColumn3
            // 
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            this.dataGridViewTextBoxColumn3.DefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridViewTextBoxColumn3.HeaderText = "L, C";
            this.dataGridViewTextBoxColumn3.Name = "dataGridViewTextBoxColumn3";
            this.dataGridViewTextBoxColumn3.ReadOnly = true;
            this.dataGridViewTextBoxColumn3.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn3.ToolTipText = "Double-click grid cell to locate in source code";
            this.dataGridViewTextBoxColumn3.Width = 50;
            // 
            // dataGridViewTextBoxColumn4
            // 
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTextBoxColumn4.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewTextBoxColumn4.HeaderText = "Error Message";
            this.dataGridViewTextBoxColumn4.Name = "dataGridViewTextBoxColumn4";
            this.dataGridViewTextBoxColumn4.ReadOnly = true;
            this.dataGridViewTextBoxColumn4.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn4.Width = 1000;
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.dataGridViewTextBoxColumn1.DataPropertyName = "State";
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            this.dataGridViewTextBoxColumn1.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridViewTextBoxColumn1.HeaderText = "Parser State";
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.ReadOnly = true;
            this.dataGridViewTextBoxColumn1.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTextBoxColumn1.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.dataGridViewTextBoxColumn1.ToolTipText = "Double-click grid cell to navigate to state details";
            this.dataGridViewTextBoxColumn1.Width = 83;
            // 
            // ruler1
            // 
            this.ruler1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ruler1.Location = new System.Drawing.Point(3, 26);
            this.ruler1.MaximumSize = new System.Drawing.Size(1073741824, 22);
            this.ruler1.MinimumSize = new System.Drawing.Size(0, 22);
            this.ruler1.Name = "ruler1";
            this.ruler1.Size = new System.Drawing.Size(821, 22);
            this.ruler1.TabIndex = 11;
            this.ruler1.Target = null;
            // 
            // SettingsBag
            // 
            this.SettingsBag.LineColor = System.Drawing.SystemColors.ControlDark;
            this.SettingsBag.Location = new System.Drawing.Point(47, 54);
            this.SettingsBag.Name = "SettingsBag";
            this.SettingsBag.Size = new System.Drawing.Size(247, 182);
            this.SettingsBag.TabIndex = 10;
            this.SettingsBag.Visible = false;
            // 
            // ProgramEditorForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(827, 455);
            this.Controls.Add(this.SettingsBag);
            this.Controls.Add(this.tableLayoutPanel1);
            this.KeyPreview = true;
            this.Name = "ProgramEditorForm";
            this.Text = "Edit code:";
            this.ResizeEnd += new System.EventHandler(this.ProgramEditorForm_ResizeEnd);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ProgramEditorForm_KeyDown);
            this.Resize += new System.EventHandler(this.ProgramEditorForm_Resize);
            this.tsTopMenu.ResumeLayout(false);
            this.tsTopMenu.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.grpCompileErrors.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.gridCompileErrors)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private FastColoredTextBoxNS.IronyFCTB editTextBox;
        private System.Windows.Forms.ToolStrip tsTopMenu;
        private System.Windows.Forms.ToolStripButton cmdSend;
        private System.Windows.Forms.ToolStripButton cmdClear;
        private System.Windows.Forms.ToolStripButton cmdLoad;
        private System.Windows.Forms.ToolStripButton cmdSave;
        private System.Windows.Forms.ToolStripButton cmdRefresh;
        private System.Windows.Forms.ToolStripButton cmdSettings;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.GroupBox grpCompileErrors;
        private System.Windows.Forms.DataGridView gridCompileErrors;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn3;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn4;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private FastColoredTextBoxNS.Ruler ruler1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.PropertyGrid SettingsBag;
    }
}