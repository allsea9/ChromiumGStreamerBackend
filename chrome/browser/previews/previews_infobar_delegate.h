// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREVIEWS_PREVIEWS_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_PREVIEWS_PREVIEWS_INFOBAR_DELEGATE_H_

#include "base/callback.h"
#include "components/infobars/core/confirm_infobar_delegate.h"

namespace content {
class WebContents;
}

// Shows an infobar that lets the user know that a preview page has been loaded,
// and gives the user a link to reload the original page. This infobar will only
// be shown once per page load. Records UMA data for user interactions with the
// infobar.
class PreviewsInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  // The type of the infobar. It controls the strings and what UMA data is
  // recorded for the infobar.
  enum PreviewsInfoBarType {
    LOFI,      // Server-side image replacement.
    LITE_PAGE, // Server-side page rewrite.
    OFFLINE,   // Offline copy of the page.
  };

  typedef base::Callback<void(bool opt_out)> OnDismissPreviewsInfobarCallback;

  // Actions on the previews infobar. This enum must remain synchronized with
  // the enum of the same name in metrics/histograms/histograms.xml.
  enum PreviewsInfoBarAction {
    INFOBAR_SHOWN = 0,
    INFOBAR_LOAD_ORIGINAL_CLICKED = 1,
    INFOBAR_DISMISSED_BY_USER = 2,
    INFOBAR_DISMISSED_BY_NAVIGATION = 3,
    INFOBAR_INDEX_BOUNDARY
  };

  ~PreviewsInfoBarDelegate() override;

  // Creates a preview infobar and corresponding delegate and adds the infobar
  // to InfoBarService.
  static void Create(
      content::WebContents* web_contents,
      PreviewsInfoBarType infobar_type,
      const OnDismissPreviewsInfobarCallback& on_dismiss_callback);

 private:
  PreviewsInfoBarDelegate(
      content::WebContents* web_contents,
      PreviewsInfoBarType infobar_type,
      const OnDismissPreviewsInfobarCallback& on_dismiss_callback);

  // ConfirmInfoBarDelegate overrides:
  infobars::InfoBarDelegate::InfoBarIdentifier GetIdentifier() const override;
  int GetIconId() const override;
  bool ShouldExpire(const NavigationDetails& details) const override;
  void InfoBarDismissed() override;
  base::string16 GetMessageText() const override;
  int GetButtons() const override;
  base::string16 GetLinkText() const override;
  bool LinkClicked(WindowOpenDisposition disposition) override;

  PreviewsInfoBarType infobar_type_;

  OnDismissPreviewsInfobarCallback on_dismiss_callback_;

  DISALLOW_COPY_AND_ASSIGN(PreviewsInfoBarDelegate);
};

#endif  // CHROME_BROWSER_PREVIEWS_PREVIEWS_INFOBAR_DELEGATE_H_
