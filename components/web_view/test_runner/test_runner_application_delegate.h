// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_VIEW__TEST_RUNNER_TEST_RUNNER_APPLICATION_DELEGATE_H_
#define COMPONENTS_WEB_VIEW__TEST_RUNNER_TEST_RUNNER_APPLICATION_DELEGATE_H_

#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "components/view_manager/public/cpp/view_tree_delegate.h"
#include "components/view_manager/public/interfaces/view_tree_host.mojom.h"
#include "components/web_view/public/cpp/web_view.h"
#include "components/web_view/public/interfaces/web_view.mojom.h"
#include "components/web_view/test_runner/public/interfaces/layout_test_runner.mojom.h"
#include "mojo/application/public/cpp/application_delegate.h"
#include "mojo/application/public/cpp/interface_factory.h"
#include "mojo/common/weak_binding_set.h"

class GURL;

namespace mojo {
class View;
}

namespace web_view {

class TestRunnerApplicationDelegate
    : public mojo::ApplicationDelegate,
      public mojo::ViewTreeDelegate,
      public mojom::WebViewClient,
      public LayoutTestRunner,
      public mojo::InterfaceFactory<LayoutTestRunner> {
 public:
  TestRunnerApplicationDelegate();
  ~TestRunnerApplicationDelegate() override;

 private:
  void LaunchURL(const GURL& test_url);
  void Terminate();

  // mojo::ApplicationDelegate:
  void Initialize(mojo::ApplicationImpl* app) override;
  bool ConfigureIncomingConnection(
      mojo::ApplicationConnection* connection) override;

  // mojo::ViewTreeDelegate:
  void OnEmbed(mojo::View* root) override;
  void OnConnectionLost(mojo::ViewTreeConnection* connection) override;

  // mojom::WebViewClient:
  void TopLevelNavigate(mojo::URLRequestPtr request) override;
  void LoadingStateChanged(bool is_loading) override;
  void ProgressChanged(double progress) override;
  void TitleChanged(const mojo::String& title) override;

  // LayoutTestRunner:
  void TestFinished() override;

  // mojo::InterfaceFactory<LayoutTestRunner>:
  void Create(mojo::ApplicationConnection* connection,
              mojo::InterfaceRequest<LayoutTestRunner> request) override;

  mojo::ApplicationImpl* app_;
  mojo::ViewTreeHostPtr host_;

  mojo::View* root_;
  mojo::View* content_;
  scoped_ptr<WebView> web_view_;

  base::CommandLine::StringVector cmdline_args_;
  size_t cmdline_position_;

  mojo::WeakBindingSet<LayoutTestRunner> layout_test_runner_;

  DISALLOW_COPY_AND_ASSIGN(TestRunnerApplicationDelegate);
};

}  // namespace web_view

#endif  // COMPONENTS_WEB_VIEW__TEST_RUNNER_TEST_RUNNER_APPLICATION_DELEGATE_H_
