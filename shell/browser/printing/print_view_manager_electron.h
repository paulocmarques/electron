// Copyright 2020 Microsoft, Inc. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ELECTRON_SHELL_BROWSER_PRINTING_PRINT_VIEW_MANAGER_ELECTRON_H_
#define ELECTRON_SHELL_BROWSER_PRINTING_PRINT_VIEW_MANAGER_ELECTRON_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/memory/ref_counted_memory.h"
#include "build/build_config.h"
#include "chrome/browser/printing/print_view_manager_base.h"
#include "components/printing/common/print.mojom.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "printing/print_settings.h"

namespace electron {

class PrintViewManagerElectron
    : public printing::PrintViewManagerBase,
      public content::WebContentsUserData<PrintViewManagerElectron> {
 public:
  enum PrintResult {
    PRINT_SUCCESS,
    PRINTING_FAILED,
    INVALID_PRINTER_SETTINGS,
    INVALID_MEMORY_HANDLE,
    METAFILE_MAP_ERROR,
    METAFILE_INVALID_HEADER,
    METAFILE_GET_DATA_ERROR,
    SIMULTANEOUS_PRINT_ACTIVE,
    PAGE_RANGE_SYNTAX_ERROR,
    PAGE_RANGE_INVALID_RANGE,
    PAGE_COUNT_EXCEEDED,
  };

  using PrintToPDFCallback =
      base::OnceCallback<void(PrintResult,
                              scoped_refptr<base::RefCountedMemory>)>;

  ~PrintViewManagerElectron() override;

  PrintViewManagerElectron(const PrintViewManagerElectron&) = delete;
  PrintViewManagerElectron& operator=(const PrintViewManagerElectron&) = delete;

  static void BindPrintManagerHost(
      mojo::PendingAssociatedReceiver<printing::mojom::PrintManagerHost>
          receiver,
      content::RenderFrameHost* rfh);

  static std::string PrintResultToString(PrintResult result);

  void PrintToPdf(content::RenderFrameHost* rfh,
                  const std::string& page_ranges,
                  printing::mojom::PrintPagesParamsPtr print_page_params,
                  PrintToPDFCallback callback);

 private:
  explicit PrintViewManagerElectron(content::WebContents* web_contents);
  friend class content::WebContentsUserData<PrintViewManagerElectron>;

  // WebContentsObserver overrides (via PrintManager):
  void RenderFrameDeleted(content::RenderFrameHost* render_frame_host) override;

  // printing::mojom::PrintManagerHost:
  void DidPrintDocument(printing::mojom::DidPrintDocumentParamsPtr params,
                        DidPrintDocumentCallback callback) override;
  void DidGetPrintedPagesCount(int32_t cookie, uint32_t number_pages) override;
  void GetDefaultPrintSettings(
      GetDefaultPrintSettingsCallback callback) override;
  void ScriptedPrint(printing::mojom::ScriptedPrintParamsPtr params,
                     ScriptedPrintCallback callback) override;
  void ShowInvalidPrinterSettingsError() override;
  void PrintingFailed(int32_t cookie,
                      printing::mojom::PrintFailureReason reason) override;
#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
  void UpdatePrintSettings(int32_t cookie,
                           base::Value::Dict job_settings,
                           UpdatePrintSettingsCallback callback) override;
  void SetupScriptedPrintPreview(
      SetupScriptedPrintPreviewCallback callback) override;
  void ShowScriptedPrintPreview(bool source_is_modifiable) override;
  void RequestPrintPreview(
      printing::mojom::RequestPrintPreviewParamsPtr params) override;
  void CheckForCancel(int32_t preview_ui_id,
                      int32_t request_id,
                      CheckForCancelCallback callback) override;
#endif

  void Reset();
  void ReleaseJob(PrintResult result);

  raw_ptr<content::RenderFrameHost> printing_rfh_ = nullptr;
  PrintToPDFCallback callback_;
  std::string data_;
  std::vector<int32_t> headless_jobs_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace electron

#endif  // ELECTRON_SHELL_BROWSER_PRINTING_PRINT_VIEW_MANAGER_ELECTRON_H_
