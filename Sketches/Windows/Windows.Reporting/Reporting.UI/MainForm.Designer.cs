namespace Reporting.UI
{
    partial class MainForm
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
            this.reportViewer = new Microsoft.Reporting.WinForms.ReportViewer();
            this.buttonPartners = new System.Windows.Forms.Button();
            this.buttonSavedPartnerReport = new System.Windows.Forms.Button();
            this.buttonSaveAsPDF = new System.Windows.Forms.Button();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.buttonSavedPartnerReportSilentPrint = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // reportViewer
            // 
            this.reportViewer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.reportViewer.Location = new System.Drawing.Point(12, 68);
            this.reportViewer.Name = "reportViewer";
            this.reportViewer.Size = new System.Drawing.Size(768, 493);
            this.reportViewer.TabIndex = 0;
            // 
            // buttonPartners
            // 
            this.buttonPartners.Location = new System.Drawing.Point(12, 12);
            this.buttonPartners.Name = "buttonPartners";
            this.buttonPartners.Size = new System.Drawing.Size(100, 50);
            this.buttonPartners.TabIndex = 1;
            this.buttonPartners.Text = "Partners";
            this.buttonPartners.UseVisualStyleBackColor = true;
            this.buttonPartners.Click += new System.EventHandler(this.buttonPartners_Click);
            // 
            // buttonSavedPartnerReport
            // 
            this.buttonSavedPartnerReport.Location = new System.Drawing.Point(118, 12);
            this.buttonSavedPartnerReport.Name = "buttonSavedPartnerReport";
            this.buttonSavedPartnerReport.Size = new System.Drawing.Size(100, 50);
            this.buttonSavedPartnerReport.TabIndex = 2;
            this.buttonSavedPartnerReport.Text = "Saved Partner Report";
            this.buttonSavedPartnerReport.UseVisualStyleBackColor = true;
            this.buttonSavedPartnerReport.Click += new System.EventHandler(this.buttonSavedPartnerReport_Click);
            // 
            // buttonSaveAsPDF
            // 
            this.buttonSaveAsPDF.Location = new System.Drawing.Point(224, 12);
            this.buttonSaveAsPDF.Name = "buttonSaveAsPDF";
            this.buttonSaveAsPDF.Size = new System.Drawing.Size(100, 50);
            this.buttonSaveAsPDF.TabIndex = 3;
            this.buttonSaveAsPDF.Text = "Download Saved Report As PDF";
            this.buttonSaveAsPDF.UseVisualStyleBackColor = true;
            this.buttonSaveAsPDF.Click += new System.EventHandler(this.buttonSaveAsPDF_Click);
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "pdf";
            this.saveFileDialog.Filter = "PDF|*.pdf";
            // 
            // buttonSavedPartnerReportSilentPrint
            // 
            this.buttonSavedPartnerReportSilentPrint.Location = new System.Drawing.Point(330, 12);
            this.buttonSavedPartnerReportSilentPrint.Name = "buttonSavedPartnerReportSilentPrint";
            this.buttonSavedPartnerReportSilentPrint.Size = new System.Drawing.Size(100, 50);
            this.buttonSavedPartnerReportSilentPrint.TabIndex = 4;
            this.buttonSavedPartnerReportSilentPrint.Text = "Saved Partner Report Silent Print";
            this.buttonSavedPartnerReportSilentPrint.UseVisualStyleBackColor = true;
            this.buttonSavedPartnerReportSilentPrint.Click += new System.EventHandler(this.buttonSavedPartnerReportSilentPrint_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.buttonSavedPartnerReportSilentPrint);
            this.Controls.Add(this.buttonSaveAsPDF);
            this.Controls.Add(this.buttonSavedPartnerReport);
            this.Controls.Add(this.buttonPartners);
            this.Controls.Add(this.reportViewer);
            this.Name = "MainForm";
            this.Text = "Report.UI";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private Microsoft.Reporting.WinForms.ReportViewer reportViewer;
        private System.Windows.Forms.Button buttonPartners;
        private System.Windows.Forms.Button buttonSavedPartnerReport;
        private System.Windows.Forms.Button buttonSaveAsPDF;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Button buttonSavedPartnerReportSilentPrint;
    }
}

